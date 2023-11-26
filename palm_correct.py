import google.generativeai as palm

palm.configure(api_key="AIzaSyCC-Fe72-Ibmd8_ArIEQHdfiZnNfJM2WK8")
if __name__ == '__main__':
    inp = input()
    response = palm.chat(prompt = [inp])
    print(response.last)
    while inp != "exit":
        response = response.reply(inp)
        print()
        print(response.last)
        print()
        inp = input()