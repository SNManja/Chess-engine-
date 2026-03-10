declare module "./engine.js" {
  // la fábrica async que exporta Emscripten con MODULARIZE=1 + EXPORT_ES6=1
  export interface EngineInstance {
    init(): void;
    isEngineWorking(): void;
    add(a: number, b: number): number;
    delete?(): void;
  }

  export interface EngineModule {
    add(a: number, b: number): number;
    Engine: {
      new (): EngineInstance;
    };
  }

  export default function createEngineModule(
    opts?: Record<string, unknown>,
  ): Promise<EngineModule>;
}
