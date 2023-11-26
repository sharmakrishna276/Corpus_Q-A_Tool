from hugchat import hugchat
from hugchat.login import Login
import sys

if __name__ == '__main__':
    sign = Login(sys.argv[1], sys.argv[2])
    cookies = sign.login()
    cookie_path_dir = "./cookies_snapshot"
    sign.saveCookiesToDir(cookie_path_dir)

    if len(sys.argv) < 4:
        print("Usage: python3 api_hugging_chat.py email password final_file.txt")
        sys.exit(1)

    with open(sys.argv[3], 'r') as f:
        query = f.read()

    chatbot = hugchat.ChatBot(cookies=cookies.get_dict())

    query_result = chatbot.query(query)
    print("Final Output\n")
    print(query_result)