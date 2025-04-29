
import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { ArrowLeft, Check } from "lucide-react";

const AuthScreen = () => {
  const [pin, setPin] = useState("");
  const navigate = useNavigate();
  const [selectedValue] = useState(
    sessionStorage.getItem("selectedValue") || "0"
  );
  
  // Simular validação de PIN - em ambiente real seria uma validação com backend
  const correctPin = "1234"; // Apenas para exemplo
  const maxLength = 4;

  const handleKeyPress = (event: React.KeyboardEvent<HTMLInputElement>) => {
    // Permitir apenas números
    if (!/[0-9]/.test(event.key) && event.key !== "Backspace" && event.key !== "Enter") {
      event.preventDefault();
      return;
    }

    // Tecla verde ou Enter para confirmar após preencher todos os dígitos
    if ((event.key === "Enter" || event.keyCode === 13) && pin.length === maxLength) {
      validatePin();
    }
  };

  const validatePin = () => {
    if (pin === correctPin) {
      navigate("/processing");
    } else {
      alert("PIN incorreto. Tente novamente.");
      setPin("");
    }
  };

  // Controlar botões do terminal PAX D230
  useEffect(() => {
    const handleGlobalKeyPress = (event: KeyboardEvent) => {
      // Tecla vermelha (ESC) para voltar
      if (event.key === "Escape" || event.keyCode === 27) {
        navigate("/portfolio");
      }
      // Tecla verde (Enter) para avançar se PIN estiver completo
      if ((event.key === "Enter" || event.keyCode === 13) && pin.length === maxLength) {
        validatePin();
      }
    };

    document.addEventListener("keydown", handleGlobalKeyPress);
    return () => document.removeEventListener("keydown", handleGlobalKeyPress);
  }, [navigate, pin]);

  // Mascaramento do PIN para exibição de asteriscos
  const maskedPin = '*'.repeat(pin.length);

  return (
    <div className="flex flex-col items-center justify-center bg-gray-50 p-4 h-full max-w-sm mx-auto rounded-lg shadow-sm border border-gray-200">
      {/* Header com botão de voltar */}
      <div className="flex items-center justify-between w-full mb-6">
        <button 
          onClick={() => navigate("/portfolio")}
          className="flex items-center text-gray-600 hover:text-gray-800"
        >
          <ArrowLeft className="w-5 h-5 mr-1" />
          <span>Voltar</span>
        </button>
        <h1 className="text-lg font-medium text-gray-800">Autenticação</h1>
      </div>
      
      {/* Informação do valor selecionado */}
      <div className="mb-6 w-full bg-white p-3 rounded-md border border-gray-200">
        <p className="text-sm text-gray-600">Valor selecionado:</p>
        <p className="text-xl font-bold text-gray-800">R$ {parseFloat(selectedValue).toFixed(2)}</p>
      </div>
      
      {/* Campo de senha */}
      <div className="w-full mb-4">
        <label className="block text-center mb-2 font-medium text-gray-700">
          Insira a senha do vendedor:
        </label>
        <input
          type="password"
          value={pin}
          onChange={(e) => setPin(e.target.value.slice(0, maxLength))}
          onKeyDown={handleKeyPress}
          className="w-full p-3 border border-gray-300 rounded-md bg-white text-lg text-center font-medium"
          maxLength={maxLength}
          autoFocus
          inputMode="numeric"
        />
        <div className="mt-2 flex justify-center">
          <div className="tracking-widest text-2xl font-bold">{maskedPin}</div>
        </div>
      </div>
      
      {/* Instruções para uso do terminal */}
      <p className="mt-4 text-xs text-center text-gray-500">
        Pressione ESC para voltar | Digite 4 dígitos e pressione ENTER para confirmar
      </p>
    </div>
  );
};

export default AuthScreen;
