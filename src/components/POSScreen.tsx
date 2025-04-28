
import React, { useState } from "react";
import PhoneInput from "./PhoneInput";
import { Phone } from "lucide-react";

const POSScreen = () => {
  const [phoneNumber, setPhoneNumber] = useState("");

  return (
    <div className="flex flex-col items-center justify-center bg-gray-50 p-4 h-full max-w-sm mx-auto rounded-lg shadow-sm border border-gray-200">
      {/* Header */}
      <div className="flex items-center justify-center mb-6 w-full">
        <Phone className="w-5 h-5 text-gray-700 mr-2" />
        <h1 className="text-xl font-medium text-gray-800">Insira seu Telefone:</h1>
      </div>
      
      {/* Phone Input Field */}
      <div className="w-full mb-8">
        <PhoneInput 
          value={phoneNumber} 
          onChange={setPhoneNumber} 
        />
      </div>
      
      {/* Display area for entered number */}
      <div className="text-sm text-gray-500 mb-2">
        {phoneNumber ? 
          "Telefone inserido:" : 
          "Aguardando entrada..."
        }
      </div>
    </div>
  );
};

export default POSScreen;
