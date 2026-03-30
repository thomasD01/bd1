import { contextBridge } from "electron";
import { SERIAL_API_KEY, UNO_API_KEY } from "../env";
import { serialApi } from "./src/serial";
import { unoApi } from "./src/uno";

contextBridge.exposeInMainWorld(UNO_API_KEY, unoApi);
contextBridge.exposeInMainWorld(SERIAL_API_KEY, serialApi);
