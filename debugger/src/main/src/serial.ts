import { ipcMain, type IpcMainEvent } from "electron/main";
import { Subject, tap } from "rxjs";
import { SerialPort } from "serialport";
import {
  SERIAL_CONNECT_KEY,
  SERIAL_CURRENT_PORT_KEY,
  SERIAL_DISCONNECT_KEY,
  SERIAL_LIST_KEY,
  UNO_SEND_KEY,
  UNO_SERIAL_DATA_KEY,
} from "../../env";
import { mainWindow } from "../main";

const serialData = new Subject<string>();
let currentPort: SerialPort | null = null;

export function registerUnoHandler(): () => void {
  ipcMain.on(UNO_SEND_KEY, (event, data) => {
    if (!currentPort || !currentPort.isOpen) {
      console.warn(
        "Attempted to send data over serial port, but no port is open",
      );
      return;
    }

    currentPort.write(data, (error) => {
      if (error) {
        console.error("Error writing to serial port:", error);
      }
    });
  });

  const subscription = serialData
    .pipe(
      tap({
        next: (data) => {
          mainWindow.webContents.send(UNO_SERIAL_DATA_KEY, data.toString());
        },
        error: (error) => {
          console.error("Error in serial data stream:", error);
        },
        complete: () => {
          console.log("Serial data stream completed");
        },
      }),
    )
    .subscribe();

  return subscription.unsubscribe.bind(subscription);
}

export function registerSerialHandler() {
  ipcMain.handle(SERIAL_CONNECT_KEY, connectHandler);
  ipcMain.handle(SERIAL_DISCONNECT_KEY, disconnectHandler);
  ipcMain.handle(SERIAL_CURRENT_PORT_KEY, currentPortHandler);
  ipcMain.handle(SERIAL_LIST_KEY, listHandler);
}

type Handler<T extends (...args: any) => any> = (
  event: IpcMainEvent,
  ...args: Parameters<T>
) => ReturnType<T>;

const connectHandler: Handler<SerialAPI["connect"]> = (_, path) => {
  return new Promise((resolve) => {
    const handleError = (error: Error) => {
      if (!error) {
        return;
      }
      currentPort?.close();
      currentPort = null;
      console.error("Error connecting to serial port:", error);
      resolve(null);
    };

    currentPort = new SerialPort(
      {
        path,
        baudRate: 115_200,
        autoOpen: false,
      },
      handleError,
    );

    const timeOut = setTimeout(() => {
      handleError(new Error("Connection timed out"));
    }, 5_000);

    currentPort?.on("data", serialData.next.bind(serialData));

    currentPort.on("error", handleError);

    currentPort.on("open", () => {
      clearTimeout(timeOut);
      SerialPort.list().then((ports) => {
        const portInfo = ports.find((port) => port.path === path);
        if (!portInfo) {
          console.error(
            "Connected to port, but failed to retrieve port info for path:",
            path,
          );
          resolve(null);
          return;
        }
        resolve(portInfo);
      });
    });

    currentPort.open(handleError);

    return;
  });
};
const currentPortHandler: Handler<SerialAPI["currentPort"]> = (event) => {
  return new Promise((resolve) => {
    if (!currentPort) {
      resolve(null);
      return;
    }
    SerialPort.list().then((ports) => {
      const portInfo = ports.find((port) => port.path === currentPort?.path);
      resolve(portInfo || null);
    });
  });
};
const disconnectHandler: Handler<SerialAPI["disconnect"]> = () => {
  return new Promise((resolve, reject) => {
    if (!currentPort) {
      resolve();
      return;
    }
    currentPort.close((error) => {
      if (error) {
        reject(error);
        return;
      }
      currentPort = null;
      resolve();
    });
  });
};
const listHandler: Handler<SerialAPI["list"]> = () => {
  return SerialPort.list();
};
