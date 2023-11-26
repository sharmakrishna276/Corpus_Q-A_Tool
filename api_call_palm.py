import google.generativeai as palm
import sys

palm.configure(api_key=sys.argv[1])
# client = OpenAI(api_key=sys.argv[1])
# model = palm.get_model('models/text-bison-safety-off')
if __name__ == '__main__':
    if len(sys.argv) < 5:
        print("Usage: python3 api_call_palm.py API_KEY num_paragraphs query.txt")
        sys.exit(1)

    #api_key = sys.argv[1]
    #raise Exception("The 'openai.api_key' option isn't read in the client API. You will need to pass it when you instantiate the client, e.g. 'OpenAI(api_key=api_key)'")  # Set the OpenAI API key

    num_paragraphs = int(sys.argv[2])

    paragraphs = []
    for i in range(num_paragraphs):
        filename = 'paragraph_' + str(i) + '.txt'
        with open(filename, 'r') as f:
            paragraphs.append("para is ")
            paragraphs.append(f.read())
            paragraphs.append('\n')

    query_file = sys.argv[3]
    with open(query_file, 'r') as f:
        query = f.read()
        paragraphs.append("query is ")
        paragraphs.append(query)
        paragraphs.append('\n')

    paragraphs = '\n'.join(paragraphs)

    response = palm.chat(prompt = [paragraphs])
    print("\nGOOGLE PaLM response received\n")
    try :
        with open(sys.argv[4], "a") as f:
            f.write(response.last)
    except TypeError :
        pass
    # print(response.last)