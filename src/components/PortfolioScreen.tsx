
import React from "react";
import { Card } from "./ui/card";
import { useNavigate } from "react-router-dom";
import { ArrowLeft } from "lucide-react";

const values = [10, 15, 20, 25, 30, 40, 50, 60, 80, 100];

const PortfolioScreen = () => {
  const navigate = useNavigate();

  const handleValueSelect = (value: number) => {
    // Armazenar o valor selecionado no sessionStorage para uso posterior
    sessionStorage.setItem("selectedValue", value.toString());
    // Navegar para a tela de autenticação
    navigate("/auth");
  };

  // Manipular a tecla vermelha (voltar) e verde (avançar) no terminal POS
  React.useEffect(() => {
    const handleKeyPress = (event: KeyboardEvent) => {
      // Tecla vermelha - ESC (27)
      if (event.key === "Escape" || event.keyCode === 27) {
        navigate("/");
      }
    };

    document.addEventListener("keydown", handleKeyPress);
    return () => document.removeEventListener("keydown", handleKeyPress);
  }, [navigate]);

  return (
    <div className="flex flex-col items-center justify-center bg-gray-50 p-4 h-full max-w-sm mx-auto rounded-lg shadow-sm border border-gray-200">
      {/* Header com botão de voltar */}
      <div className="flex items-center justify-between w-full mb-4">
        <button 
          onClick={() => navigate("/")}
          className="flex items-center text-gray-600 hover:text-gray-800"
        >
          <ArrowLeft className="w-5 h-5 mr-1" />
          <span>Voltar</span>
        </button>
        <h1 className="text-lg font-medium text-gray-800">Valores Disponíveis</h1>
      </div>
      
      <div className="grid grid-cols-2 gap-3 w-full">
        {values.sort((a, b) => a - b).map((value) => (
          <Card 
            key={value} 
            className="p-3 flex items-center justify-center cursor-pointer hover:bg-gray-100 active:bg-gray-200 transition-colors"
            onClick={() => handleValueSelect(value)}
            tabIndex={0}
          >
            <span className="text-lg font-semibold text-gray-700">
              R$ {value.toFixed(2)}
            </span>
          </Card>
        ))}
      </div>
      
      {/* Instrução para uso do terminal PAX */}
      <p className="mt-4 text-xs text-center text-gray-500">
        Pressione ESC para voltar | Toque para selecionar um valor
      </p>
    </div>
  );
};

export default PortfolioScreen;
