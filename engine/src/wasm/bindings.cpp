#include <memory>
#include <string>
#include <emscripten/bind.h>

// Incluye tu header real
#include "../core/engine.hpp" // ajusta la ruta si hace falta

using namespace emscripten;

// Wrapper ligero alrededor de tu Engine C++.
// Reemplaza los métodos con las llamadas reales de tu clase Engine.
class EngineWrapper {
public:
    EngineWrapper() { }

    void isEngineWorking(){
        // imprime para verificar desde node/browser console (stdout)
        printf("Engine is working correctly\n");
    }

    int add(int a, int b) {
        // usa la función existente engine::add
        return engine::add(a, b);
    }

    void init() {
        // TODO: inicializar motor
        // engine_->init();
    }
private:
};

// Registrar con embind
EMSCRIPTEN_BINDINGS(chess_engine_module) {
    // exporta la clase
    class_<EngineWrapper>("Engine")
        .constructor<>()
        .function("init", &EngineWrapper::init)
        .function("isEngineWorking", &EngineWrapper::isEngineWorking)
        .function("add", &EngineWrapper::add)
        ;

    // exporta la función libre definida en src/core/engine.cpp
    function("add", &engine::add);
}