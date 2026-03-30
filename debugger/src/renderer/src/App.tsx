import React from "react";
import { ThemeProvider } from "./components/theme-provider";
import { AppContextProvider } from "./context";
import MainPage from "./pages/main.page";

export default function App() {
  return (
    <ThemeProvider>
      <AppContextProvider>
        <MainPage />
      </AppContextProvider>
    </ThemeProvider>
  );
}
