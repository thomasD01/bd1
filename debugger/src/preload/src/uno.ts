import { ipcRenderer } from "electron";
import { UNO_SEND_KEY, UNO_SERIAL_DATA_KEY } from "../../env";

export const unoApi: UnoAPI = {
  send: (data: any) => {
    ipcRenderer.send(UNO_SEND_KEY, data);
  },
  serialData: (callback: (data: string) => void) => {
    const subscription = (event: Electron.IpcRendererEvent, data: string) => {
      callback(data);
    };
    ipcRenderer.on(UNO_SERIAL_DATA_KEY, subscription);
    return () => {
      ipcRenderer.removeListener(UNO_SERIAL_DATA_KEY, subscription);
    };
  },
};
