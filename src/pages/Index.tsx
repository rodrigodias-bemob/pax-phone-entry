
import POSScreen from "../components/POSScreen";

const Index = () => {
  return (
    <div className="min-h-screen flex items-center justify-center bg-gray-100 p-4">
      <div className="w-full max-w-sm">
        {/* POSScreen com aspecto próximo ao de um terminal de pagamento */}
        <div className="bg-black p-3 rounded-t-2xl">
          <div className="h-1 w-16 bg-gray-700 mx-auto rounded-full mb-1" />
        </div>
        <POSScreen />
      </div>
    </div>
  );
};

export default Index;
