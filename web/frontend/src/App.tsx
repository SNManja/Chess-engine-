import { useEffect, useState } from "react";
import { Chessboard } from "react-chessboard";
import "./App.css";
import createEngineModule from "./wasm/engine.js";

function App() {
  const [wasmLoaded, setWasmLoaded] = useState(false);
  const [sumResult, setSumResult] = useState<number | null>(null);
  const chessboardOptions = {};
  useEffect(() => {
    const initWasm = async () => {
      const { instance } = await createEngineModule();
      setWasmLoaded(true);
      setSumResult(instance.exports.add(2, 3));
    };
    initWasm();
  }, []);

  return (
    <div id="app">
      <div id="header">
        <h1 id="header-title">Chess engine</h1>
        <p id="header-wasm-info">
          {wasmLoaded && sumResult !== null
            ? `Wasm in working`
            : "Cargando WASM..."}
        </p>
      </div>
      <div className="chessboard">
        <Chessboard options={chessboardOptions} />
      </div>
    </div>
  );
}

export default App;
