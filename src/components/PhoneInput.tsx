
import React, { useState } from "react";

interface PhoneInputProps {
  value: string;
  onChange: (value: string) => void;
}

const PhoneInput = ({ value, onChange }: PhoneInputProps) => {
  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    // Remove todos os caracteres não numéricos
    const numericValue = e.target.value.replace(/\D/g, "");
    
    // Limitar a 11 dígitos (DDD + número)
    const limitedValue = numericValue.slice(0, 11);
    
    // Aplicar máscara (xx)xxxxx-xxxx
    let formattedValue = "";
    
    if (limitedValue.length > 0) {
      // Adiciona parênteses para o DDD
      formattedValue = `(${limitedValue.slice(0, 2)}`;
      
      if (limitedValue.length > 2) {
        // Adiciona o fechamento dos parênteses e os próximos dígitos
        formattedValue += `)${limitedValue.slice(2, 7)}`;
        
        if (limitedValue.length > 7) {
          // Adiciona o hífen e os últimos dígitos
          formattedValue += `-${limitedValue.slice(7)}`;
        }
      }
    }
    
    onChange(formattedValue);
  };
  
  // Manipular eventos de tecla para terminais POS
  const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    // Permitir apenas dígitos, backspace e teclas de navegação
    if (
      !/[0-9]/.test(e.key) && 
      e.key !== "Backspace" && 
      e.key !== "Delete" && 
      e.key !== "ArrowLeft" && 
      e.key !== "ArrowRight" &&
      e.key !== "Enter"
    ) {
      e.preventDefault();
    }
  };

  return (
    <input
      type="tel"
      value={value}
      onChange={handleChange}
      onKeyDown={handleKeyDown}
      className="w-full p-3 border border-gray-300 rounded-md bg-white text-lg text-center font-medium"
      placeholder="(xx)xxxxx-xxxx"
      inputMode="numeric"
      autoFocus
    />
  );
};

export default PhoneInput;
