import sys

def main():
    if len(sys.argv) < 2:
        print("Python analyze: please provide a mode or argument.")
        return

    mode = sys.argv[1]

    if mode == "hello":
        print("Hello from Python, called by C shell!")
    elif mode == "len" and len(sys.argv) > 2:
        s = sys.argv[2]
        print(f"Length of '{s}' is {len(s)}")
    else:
        print("Unknown mode:", mode)

if __name__ == "__main__":
    main()
