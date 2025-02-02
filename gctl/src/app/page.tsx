"use client";

import { AnimatePresence, motion } from "framer-motion";
import { useEffect, useState } from "react";
import useSWR from "swr";

const fetcher = (url: string) => fetch(url).then((res) => res.json());

export default function Home() {
  const { data, mutate } = useSWR("/api/message", fetcher);
  const [messages, setMessages] = useState<string[]>([]);

  // Atualiza as mensagens locais sempre que SWR receber novos dados
  useEffect(() => {
    if (data?.messages) {
      setMessages(data.messages); // Agora, a API já limita para 2 mensagens
    }
  }, [data]); // Atualiza as mensagens somente quando os dados mudam

  useEffect(() => {
    // Faz a atualização da lista automaticamente a cada 5 segundos
    const interval = setInterval(() => {
      mutate(); // Força o SWR a revalidar os dados
    }, 5000); // 5000ms = 5 segundos

    return () => clearInterval(interval); // Limpa o intervalo ao desmontar o componente
  }, [mutate]);

  return (
    <div className="min-h-screen flex flex-col items-center justify-between bg-gray-100">
      <header className="w-screen flex flex-col md:flex-row items-center justify-center gap-4 md:gap-40 m-10 p-2">
        <ul className="w-full md:w-4/5 flex flex-col md:flex-row list-none justify-between gap-4">
          <li className="flex items-center justify-center">
            <img className="max-w-24" src="logotipo-UFV1.png" alt="UFV Logo" />
          </li>
          <li className="flex items-center justify-center">
            <img className="max-w-32" src="iot-Image.png" alt="IoT img" />
          </li>
          <li className="flex items-center justify-center">
            <h2 className="text-xl md:text-2xl font-bold text-gray-700 text-center md:text-left">
              Trabalho Prático - CCF 451 - Internet das Coisas
            </h2>
          </li>
        </ul>
      </header>

      <div className="w-full flex flex-col items-center justify-center mb-64">
        <h1 className="text-3xl font-bold text-gray-700 mb-4">
          Mensagens Recebidas
        </h1>
        <ol className="w-2/5 bg-white shadow-lg rounded-lg p-4 overflow-hidden">
          <AnimatePresence>
            {messages.map((msg, index) => (
              <motion.li
                key={msg + index}
                initial={{ opacity: 0, y: -10 }}
                animate={{ opacity: 1, y: 0 }}
                exit={{ opacity: 0, y: 10 }}
                transition={{ duration: 0.5 }}
                className="text-justify font-bold border-b last:border-none p-2 text-gray-500"
              >
                {index + 1}- {msg == "Noisy" ? "Ambiente inadequado no momento..." : "O ambiente está ótimo e adequado!"}
              </motion.li>
            ))}
          </AnimatePresence>
        </ol>
      </div>
    </div>
  );
}
