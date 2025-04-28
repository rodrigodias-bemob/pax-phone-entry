
import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import PhoneInput from "./PhoneInput";
import { Phone } from "lucide-react";
import { detectCarrier } from "../utils/phoneUtils";
import { Avatar, AvatarImage, AvatarFallback } from "./ui/avatar";

const carrierLogos = {
  TIM: "https://logodownload.org/wp-content/uploads/2014/02/tim-logo-1.png",
  VIVO: "https://logodownload.org/wp-content/uploads/2014/02/vivo-logo-0.png",
  CLARO: "https://logodownload.org/wp-content/uploads/2014/02/claro-logo-0.png",
  OI: "https://logodownload.org/wp-content/uploads/2014/02/oi-logo-0.png"
};

const POSScreen = () => {
  const [phoneNumber, setPhoneNumber] = useState("");
  const navigate = useNavigate();
  const carrier = detectCarrier(phoneNumber);

  useEffect(() => {
    const handleKeyPress = (event: KeyboardEvent) => {
      if (event.key === "Enter" && phoneNumber.replace(/\D/g, "").length >= 10) {
        navigate("/portfolio");
      }
    };

    document.addEventListener("keydown", handleKeyPress);
    return () => document.removeEventListener("keydown", handleKeyPress);
  }, [phoneNumber, navigate]);

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
      
      {/* Display area for carrier logo or status */}
      <div className="flex flex-col items-center">
        {carrier ? (
          <Avatar className="w-24 h-12 rounded-none bg-transparent">
            <AvatarImage 
              src={carrierLogos[carrier]} 
              alt={carrier} 
              className="object-contain"
            />
            <AvatarFallback>{carrier}</AvatarFallback>
          </Avatar>
        ) : (
          <div className="text-sm text-gray-500">
            {phoneNumber ? "Identificando operadora..." : "Aguardando entrada..."}
          </div>
        )}
      </div>
    </div>
  );
};

export default POSScreen;
