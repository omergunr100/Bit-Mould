from os import listdir, path, write
import imageio

def main():
    file_dir = path.realpath(path.dirname(__file__))
    directory = path.join(file_dir, "/images/")
    images = []
    gif_path = path.join(file_dir,"gif.gif")
    writer = imageio.get_writer(gif_path,mode='I',duration=0.06)
    files = [path.join(directory, fi) for fi in listdir(directory)]
    files.sort(key=path.getmtime)
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(directory, f))
        writer.append_data(image)
    files.sort(key=path.getmtime,reverse=True)
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(directory, f))
        writer.append_data(image)
    writer.close()
        

if __name__ == "__main__":
    main()
