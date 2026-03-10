#include "core/engine.hpp"
#include <cstdint>

using Bitboard = uint64_t;

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

enum square {
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

inline void set_bit(Bitboard &board, int square){
    board |= (1ULL << square); 
}


inline void pop_bit(Bitboard &board, int square){
    board &= ~(1ULL << square);
}

inline bool get_bit(Bitboard &board, int square){
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


void init_board() {
    white_pawns   = 0x000000000000FF00ULL;
    white_rooks   = 0x0000000000000081ULL;
    white_knights = 0x0000000000000042ULL;
    white_bishops = 0x0000000000000024ULL;
    white_queens  = 0x0000000000000008ULL;
    white_king    = 0x0000000000000010ULL;

    black_pawns   = 0x00FF000000000000ULL;
    black_rooks   = 0x8100000000000000ULL;
    black_knights = 0x4200000000000000ULL;
    black_bishops = 0x2400000000000000ULL;
    black_queens  = 0x0800000000000000ULL;
    black_king    = 0x1000000000000000ULL;

    white_pieces = white_pawns | white_rooks | white_knights | white_bishops | white_queens | white_king;
    black_pieces = black_pawns | black_rooks | black_knights | black_bishops | black_queens | black_king;
    all_pieces   = white_pieces | black_pieces;
}

//Dada una posicion, devuelve los movimientos posibles de una pieza. Se excluyen las posiciones donde haya una pieza aliada del jugador que realiza el movimiento. Para evitar codigo repetido en ambos colores parametrizo el bitboard de las piezas propias.
Bitboard knight_attacks(square square, Bitboard own_pieces){
    Bitboard knight = 1ULL << square;
    return (knight << 17 & ~FILE_A) |
            (knight << 15 & ~FILE_H) |
            (knight >> 17 & ~FILE_H) |
            (knight >> 15 & ~FILE_A) |
            (knight << 10 & ~FILE_AB) |
            (knight >> 6 & ~FILE_AB) |
            (knight << 6 & ~FILE_GH) |
            (knight >> 10 & ~FILE_GH) |
            & ~own_pieces;
}


namespace engine {
    int add(int a, int b) {
        return a + b;
    }
}