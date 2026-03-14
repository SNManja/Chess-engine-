#include "core/engine.hpp"
#include <cstdint>

using Bitboard = uint64_t;

enum square {
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, 
    NO_SQUARE = -1
};


inline void set_bit(Bitboard &board, square square){
    board |= (1ULL << square); 
}


inline void pop_bit(Bitboard &board, square square){
    board &= ~(1ULL << square);
}

inline bool get_bit(Bitboard board, square square){
    return board & (1ULL << square);
}


constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = 0x0202020202020202ULL;
constexpr Bitboard FILE_H = 0x8080808080808080ULL;
constexpr Bitboard FILE_G = 0x4040404040404040ULL;
constexpr Bitboard FILE_AB = FILE_A | FILE_B;
constexpr Bitboard FILE_GH = FILE_G | FILE_H;

constexpr Bitboard RANK_1 = 0x00000000000000FFULL;
constexpr Bitboard RANK_8 = 0xFF00000000000000ULL;

struct board{

    Bitboard white_pawns;
    Bitboard white_knights;
    Bitboard white_bishops;
    Bitboard white_rooks;
    Bitboard white_queens;
    Bitboard white_king;

    Bitboard black_pawns;
    Bitboard black_knights;
    Bitboard black_bishops;
    Bitboard black_rooks;
    Bitboard black_queens;
    Bitboard black_king;

    Bitboard white_pieces;
    Bitboard black_pieces;

    Bitboard all_pieces;

    bool     white_to_move; // es true si mueven blancas, false si mueven negras
    uint8_t  castling_rights; // 4 bits para ver que enroques se pueden hacer
    square   en_passant_square;  // NO_SQUARE si no hay peón al paso disponible
    int      halfmove_clock;     // para la regla de 50 movimientos
    int      fullmove_number;   // numero de jugada


    void update_occupancy() {
    white_pieces = white_pawns | white_rooks | white_knights | white_bishops | white_queens | white_king;
    black_pieces = black_pawns | black_rooks | black_knights | black_bishops | black_queens | black_king;
    all_pieces   = white_pieces | black_pieces;
    }
};



void init_board() {
    board b{};

    b.white_pawns   = 0x000000000000FF00ULL;
    b.white_rooks   = 0x0000000000000081ULL;
    b.white_knights = 0x0000000000000042ULL;
    b.white_bishops = 0x0000000000000024ULL;
    b.white_queens  = 0x0000000000000008ULL;
    b.white_king    = 0x0000000000000010ULL;

    b.black_pawns   = 0x00FF000000000000ULL;
    b.black_rooks   = 0x8100000000000000ULL;
    b.black_knights = 0x4200000000000000ULL;
    b.black_bishops = 0x2400000000000000ULL;
    b.black_queens  = 0x0800000000000000ULL;
    b.black_king    = 0x1000000000000000ULL;

    b.update_occupancy();

    b.white_to_move     = true;
    b.castling_rights   = 0b1111;
    b.en_passant_square = NO_SQUARE;
    b.halfmove_clock    = 0;
    b.fullmove_number   = 1;

    b.white_pieces = b.white_pawns | b.white_rooks | b.white_knights | b.white_bishops | b.white_queens | b.white_king;
    b.black_pieces = b.black_pawns | b.black_rooks | b.black_knights | b.black_bishops | b.black_queens | b.black_king;
    b.all_pieces   = b.white_pieces | b.black_pieces;
}

/*Dada una posicion, devuelve los movimientos posibles de una pieza. Se excluyen las posiciones 
donde haya una pieza aliada del jugador que realiza el movimiento. Para evitar codigo repetido 
en ambos colores parametrizo el bitboard de las piezas propias.*/
Bitboard knight_moves(square square, Bitboard own_pieces){
    Bitboard knight = 1ULL << square;
    Bitboard moves =
        ((knight << 17) & ~FILE_A) |
        ((knight << 15) & ~FILE_H) |
        ((knight >> 17) & ~FILE_H) |
        ((knight >> 15) & ~FILE_A) |
        ((knight << 10) & ~FILE_AB) |
        ((knight >> 6)  & ~FILE_AB) |
        ((knight << 6)  & ~FILE_GH) |
        ((knight >> 10) & ~FILE_GH);

    return moves & ~own_pieces;
}

Bitboard king_moves(square sq, Bitboard own_pieces) { //FALTA VER ENROQUE Y QUE UN REY NO PUEDE MOVERSE A POSICIONES QUE ESTEN SIENDO ATACADAS
    Bitboard king = 1ULL << sq;
    Bitboard moves =
        (king << 8)           | 
        (king >> 8)           |
        ((king << 1) & ~FILE_A) |  
        ((king >> 1) & ~FILE_H) | 
        ((king << 9) & ~FILE_A) |  
        ((king << 7) & ~FILE_H) | 
        ((king >> 7) & ~FILE_A) | 
        ((king >> 9) & ~FILE_H);  

    return moves & ~own_pieces;
}


Bitboard bishop_moves(square sq, Bitboard own_pieces, Bitboard all_pieces, Bitboard enemy_pieces){
    Bitboard bishop = 1ULL << sq;
    Bitboard moves = 0ULL;

    Bitboard diag = bishop << 9; //diag derecha arriba
    while(diag & ~FILE_A){ 
        if(diag & all_pieces){
            moves |= diag & enemy_pieces;
            break;
        }
        moves |= diag;
        diag <<= 9;
    }

    diag = bishop << 7; //diag izquierda arriba
    while(diag & ~FILE_H){ 
        if(diag & all_pieces){
            moves |= diag & enemy_pieces;
            break;
        }
        moves |= diag;
        diag <<= 7;
    }

    diag = bishop >> 9; //diag izquierda abajo
    while(diag & ~FILE_H){ 
        if(diag & all_pieces){
            moves |= diag & enemy_pieces;
            break;
        }
        moves |= diag;
        diag >>= 9;
    }

    diag = bishop >> 7; //diag derecha abajo
    while(diag & ~FILE_A){ 
        if(diag & all_pieces){
            moves |= diag & enemy_pieces;
            break;
        }
        moves |= diag;
        diag >>= 7;
    }

    return moves;

}

Bitboard white_pawn_moves(Bitboard pawns, Bitboard all_pieces, 
                          Bitboard enemy_pieces, square ep_square) {
    Bitboard moves = 0ULL;
    Bitboard empty = ~all_pieces; //quedan en 1 las casillas sin piezas
    
    // avance simple
    Bitboard push1 = (pawns << 8) & empty; //uso empty para ver que no haya una pieza arriba del peon
    moves |= push1;

    //avance doble
    //solo lo puedo hacer desde la segunda fila.
    constexpr Bitboard RANK_2 = 0x000000000000FF00ULL;
    Bitboard push2 = ((pawns & RANK_2) << 16) & empty & (empty << 8); //pawns & RANK_2 se queda solo con los peones de la fila 2
    moves |= push2;
    
    //diagonales
    Bitboard captura_derecha = (pawns << 9) & ~FILE_A & enemy_pieces;
    Bitboard captura_izquierda = (pawns << 7) &  ~FILE_H & enemy_pieces;
    moves |= captura_derecha | captura_izquierda;

    // captura al paso 
    if (ep_square != NO_SQUARE) {
        Bitboard ep = 1ULL << ep_square;
        moves |= ((pawns << 9) & ~FILE_A & ep);
        moves |= ((pawns << 7) & ~FILE_H & ep);
    }

   return moves;
}

Bitboard black_pawn_moves(Bitboard pawns, Bitboard all_pieces, 
                          Bitboard enemy_pieces, square ep_square) {
    Bitboard moves = 0ULL;
    Bitboard empty = ~all_pieces; //quedan en 1 las casillas sin piezas
    
    // avance simple
    Bitboard push1 = (pawns >> 8) & empty; //uso empty para ver que no haya una pieza arriba del peon
    moves |= push1;

    //avance doble
    //solo lo puedo hacer desde la septima fila.
    constexpr Bitboard RANK_7 = 0x00FF000000000000ULL;
    Bitboard push2 = ((pawns & RANK_7) >> 16) & empty & (empty >> 8); //pawns & RANK_7 se queda solo con los peones de la fila 7
    moves |= push2;
    
    //diagonales
    Bitboard captura_derecha = (pawns >> 9) & ~FILE_A & enemy_pieces;
    Bitboard captura_izquierda = (pawns >> 7) &  ~FILE_H & enemy_pieces;
    moves |= captura_derecha | captura_izquierda;

        // captura al paso
    if (ep_square != NO_SQUARE) {
        Bitboard ep = 1ULL << ep_square;
        moves |= ((pawns >> 9) & ~FILE_A & ep);
        moves |= ((pawns >> 7) & ~FILE_H & ep);
    }

   return moves;
}
namespace engine {
    int add(int a, int b) {
        return a + b;
    }
}