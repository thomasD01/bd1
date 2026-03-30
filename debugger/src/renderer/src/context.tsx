import React from "react";
import "../../env";

type AppContext = {
  currentPort: any | null;
  ports: any[];
  scanForPorts: () => Promise<void>;
  connectToPort: (path: string) => Promise<void>;
};

const appContext = React.createContext<AppContext>(null!!);

export const useAppContext = () => React.useContext(appContext);
export function AppContextProvider(props: React.PropsWithChildren) {
  const [ports, setPorts] = React.useState<any[]>([]);
  const [currentPort, setCurrentPort] = React.useState<any>(null);

  async function scanForPorts() {
    return window.serial.list().then(setPorts);
  }

  async function connectToPort(path: string) {
    return window.serial.connect(path).then(setCurrentPort);
  }

  window.uno.serialData((data) => {
    console.log("Received data from Uno serial:", data);
  });

  React.useEffect(() => {
    window.serial.currentPort().then(setCurrentPort);
  }, []);

  return (
    <appContext.Provider
      value={{
        currentPort,
        ports,
        scanForPorts,
        connectToPort,
      }}
    >
      {props.children}
    </appContext.Provider>
  );
}
