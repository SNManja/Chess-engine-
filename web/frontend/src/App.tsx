import { useEffect, useState } from "react";
import "./App.css";
import reactLogo from "./assets/react.svg";
import createEngineModule from "./wasm/engine.js";
import viteLogo from "/vite.svg";

function App() {
  const [count, setCount] = useState(0);
  const [wasmLoaded, setWasmLoaded] = useState(false);
  const [sumResult, setSumResult] = useState<number | null>(null);

  useEffect(() => {
    let cancelled = false;

    (async () => {
      try {
        const Module = await createEngineModule({
          locateFile: (path: string) => {
            if (path.endsWith(".wasm")) {
              return "/src/wasm/engine.wasm";
            }
            return path;
          },
        });

        if (cancelled) return;

        const s = Module.add(2, 3);
        setSumResult(s);

        const engineInstance = new Module.Engine();
        engineInstance.isEngineWorking();
        console.log("wrapper add:", engineInstance.add(4, 5));

        setWasmLoaded(true);
      } catch (err) {
        console.error("WASM load error:", err);
      }
    })();

    return () => {
      cancelled = true;
    };
  }, []);

  return (
    <>
      <div>
        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>

      <h1>Vite + React</h1>

      <div className="card">
        <button onClick={() => setCount((c) => c + 1)}>count is {count}</button>

        <p>
          Edit <code>src/App.tsx</code> and save to test HMR
        </p>

        <p>
          {wasmLoaded
            ? `WASM ready — add(2,3) = ${sumResult}`
            : "Cargando WASM..."}
        </p>
      </div>

      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
    </>
  );
}

export default App;
