import React from "react";
import { ModeToggle } from "./components/mode-toggle";
import { ThemeProvider } from "./components/theme-provider";

export default function App() {
  return (
    <ThemeProvider>
      <h1>BD-1 Debugger</h1>
      <ModeToggle />
    </ThemeProvider>
  );
}
