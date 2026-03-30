import { Loader2 } from "lucide-react";
import React from "react";

import { Button } from "../components/ui/button";
import {
  Card,
  CardContent,
  CardFooter,
  CardHeader,
  CardTitle,
} from "../components/ui/card";
import { useAppContext } from "../context";

type IProps = {};

export default function MainPage(props: React.PropsWithChildren<IProps>) {
  const [isScanning, setIsScanning] = React.useState(false);
  const {
    currentPort,
    ports,
    scanForPorts: _scanForPorts,
    connectToPort,
  } = useAppContext();

  async function scanForPorts() {
    setIsScanning(true);
    return _scanForPorts().finally(() => setIsScanning(false));
  }

  React.useEffect(() => {
    scanForPorts();
  }, []);

  if (!!currentPort) {
    return (
      <div className="w-full h-full">
        <Card>
          <h2>Connected to {currentPort.path}</h2>
          <pre>{JSON.stringify(currentPort, null, 2)}</pre>
        </Card>
      </div>
    );
  }

  return (
    <div className="w-dvw h-dvh p-8 flex flex-col items-center justify-center">
      <Card>
        <CardHeader>
          <CardTitle>Serial Port Debugger</CardTitle>
        </CardHeader>
        <CardContent>
          <ul>
            {ports.map((port) => (
              <li
                key={port.path}
                className="my-2"
                onClick={() => connectToPort(port.path)}
              >
                {port.friendlyName || port.path}
              </li>
            ))}
          </ul>
        </CardContent>
        <CardFooter>
          <div className="w-full flex flex-row">
            <div className="flex-1" />
            <Button
              onClick={scanForPorts}
              disabled={isScanning}
              variant="outline"
            >
              Scan for Ports {isScanning && <Loader2 data-icon="inline-end" />}
            </Button>
          </div>
        </CardFooter>
      </Card>
    </div>
  );
}
