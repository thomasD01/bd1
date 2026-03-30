import { type PortInfo } from "@serialport/bindings-interface";

export const UNO_API_KEY = "uno";
export const UNO_SEND_KEY = "uno-send";
export const UNO_SERIAL_DATA_KEY = "uno-serial-data";

export const SERIAL_API_KEY = "serial";
export const SERIAL_LIST_KEY = "serial-list";
export const SERIAL_CONNECT_KEY = "serial-connect";
export const SERIAL_DISCONNECT_KEY = "serial-disconnect";
export const SERIAL_CURRENT_PORT_KEY = "serial-current-port";

declare global {
  interface Window {
    [UNO_API_KEY]: UnoAPI;
    [SERIAL_API_KEY]: SerialAPI;
  }

  type UnoAPI = {
    send: (data: any) => void;
    serialData: (callback: (data: string) => void) => () => void;
  };

  type SerialAPI = {
    list: () => Promise<PortInfo[]>;
    connect: (path: string) => Promise<PortInfo>;
    disconnect: () => Promise<void>;
    currentPort: () => Promise<PortInfo | null>;
  };
}
