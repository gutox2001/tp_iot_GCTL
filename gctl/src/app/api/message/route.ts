import { NextResponse } from "next/server";

let messages: string[] = [];
function updateMessages(newMessage: string) {
    if (messages.length >= 2) {
        messages.pop(); // Remove the oldest message
    }
    messages.unshift(newMessage); // Add the new message at the beginning
}

export async function POST(req: Request) {
    try {
        const { message } = await req.json();
        if (!message) return NextResponse.json({ error: "Message is required" }, { status: 400 });

        updateMessages(message); // Armazena a mensagem
        return NextResponse.json({ success: true, messages });
    } catch (error) {
        return NextResponse.json({ error: "Invalid request" }, { status: 500 });
    }
}

export async function GET() {
    return NextResponse.json({ messages });
}
