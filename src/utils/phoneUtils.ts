
type Carrier = 'TIM' | 'VIVO' | 'CLARO' | 'OI' | undefined;

export const detectCarrier = (phoneNumber: string): Carrier => {
  // Remove any non-digit characters
  const cleanNumber = phoneNumber.replace(/\D/g, '');
  
  // Check if we have enough digits to detect carrier (needs DDD + first digit)
  if (cleanNumber.length < 3) return undefined;
  
  // Get the first digit after DDD
  const firstDigit = cleanNumber.substring(2, 3);
  
  // Brazilian carrier prefixes
  switch (firstDigit) {
    case '6':
    case '8':
    case '9':
      return 'VIVO';
    case '7':
      return 'OI';
    case '2':
    case '3':
    case '4':
      return 'CLARO';
    case '5':
      return 'TIM';
    default:
      return undefined;
  }
};
