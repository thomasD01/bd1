import { ipcRenderer } from "electron";
import {
  SERIAL_CONNECT_KEY,
  SERIAL_CURRENT_PORT_KEY,
  SERIAL_DISCONNECT_KEY,
  SERIAL_LIST_KEY,
} from "../../env";

export const serialApi: SerialAPI = {
  list: () => ipcRenderer.invoke(SERIAL_LIST_KEY),
  connect: (port: string) => ipcRenderer.invoke(SERIAL_CONNECT_KEY, port),
  disconnect: () => ipcRenderer.invoke(SERIAL_DISCONNECT_KEY),
  currentPort: () => ipcRenderer.invoke(SERIAL_CURRENT_PORT_KEY),
};
