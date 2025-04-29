
import React, { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { Check } from "lucide-react";

const ProcessingScreen = () => {
  const [isProcessing, setIsProcessing] = useState(true);
  const navigate = useNavigate();
  const [selectedValue] = useState(
    sessionStorage.getItem("selectedValue") || "0"
  );

  // Simular processamento de recarga
  useEffect(() => {
    const timer = setTimeout(() => {
      setIsProcessing(false);
    }, 2000); // 2 segundos para simular processamento

    return () => clearTimeout(timer);
  }, []);

  // Voltar para a tela inicial após um tempo
  useEffect(() => {
    if (!isProcessing) {
      const timer = setTimeout(() => {
        navigate("/");
      }, 5000); // 5 segundos na tela de sucesso antes de voltar

      return () => clearTimeout(timer);
    }
  }, [isProcessing, navigate]);

  return (
    <div className="flex flex-col items-center justify-center bg-gray-50 p-4 h-full max-w-sm mx-auto rounded-lg shadow-sm border border-gray-200">
      {isProcessing ? (
        // Tela de processamento
        <>
          <div className="mb-4">
            <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-gray-800"></div>
          </div>
          <h2 className="text-lg font-medium text-gray-800 mb-2">Processando recarga</h2>
          <p className="text-sm text-gray-600 text-center">
            Por favor, aguarde enquanto processamos sua recarga de R$ {parseFloat(selectedValue).toFixed(2)}
          </p>
        </>
      ) : (
        // Tela de sucesso
        <>
          <div className="mb-4 bg-green-100 rounded-full p-3">
            <Check className="h-12 w-12 text-green-600" />
          </div>
          <h2 className="text-lg font-medium text-gray-800 mb-2">Recarga concluída com sucesso</h2>
          <div className="mb-4 w-full bg-white p-3 rounded-md border border-gray-200">
            <p className="text-sm text-gray-600">Valor da recarga:</p>
            <p className="text-xl font-bold text-gray-800">R$ {parseFloat(selectedValue).toFixed(2)}</p>
          </div>
          <p className="text-xs text-gray-500 mt-4">
            Esta tela fechará automaticamente em alguns segundos...
          </p>
        </>
      )}
    </div>
  );
};

export default ProcessingScreen;
