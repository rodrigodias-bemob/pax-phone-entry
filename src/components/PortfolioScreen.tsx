
import React from "react";
import { Card } from "./ui/card";

const values = [10, 15, 20, 25, 30, 40, 50, 60, 80, 100];

const PortfolioScreen = () => {
  return (
    <div className="flex flex-col items-center justify-center bg-gray-50 p-4 h-full max-w-sm mx-auto rounded-lg shadow-sm border border-gray-200">
      <h1 className="text-xl font-medium text-gray-800 mb-6">Valores Disponíveis</h1>
      
      <div className="grid grid-cols-2 gap-4 w-full">
        {values.sort((a, b) => a - b).map((value) => (
          <Card key={value} className="p-4 flex items-center justify-center">
            <span className="text-xl font-semibold text-gray-700">
              R$ {value.toFixed(2)}
            </span>
          </Card>
        ))}
      </div>
    </div>
  );
};

export default PortfolioScreen;
