from os import listdir, path, write
import imageio

def main():
    file_dir = path.join("C:/Projects/C++/bit-mould/Images/")
    images = []
    gif_path = path.join(file_dir,"gif.gif")
    writer = imageio.get_writer(gif_path,mode='I',duration=0.06)
    files = [path.join(file_dir, fi) for fi in listdir(file_dir)]
    files.sort(key=path.getmtime)
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(file_dir, f))
        writer.append_data(image)
    files.sort(key=path.getmtime,reverse=True)
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(file_dir, f))
        writer.append_data(image)
    writer.close()
        

if __name__ == "__main__":
    main()
