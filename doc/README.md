# Lazespell
> Linux spell check widget for the lazy

![Lazespell Example](/doc/laze_example.png)

### Build
Lazespell requires `aspell` and `gtk-3.0` libraries and development headers.
```shell
git clone https://github.com/ryandoyle/lazespell.git
cd lazespell
cmake .
make
```

### Install
```shell
sudo make install
```
or put it somewhere on your path (assuming `$HOME/.bin/` is on your path)
```shell
cp lazespell $HOME/.bin/
```

### Running
Create a keyboard shortcut in your window manager to launch `lazespell`. (I use `Super` + `s`)
- Start typing to get spelling suggestions
- Select the suggestion using your arrow keys and press `Enter`
- Press `Enter` again to copy the word to the clipboard and close Lazespell.
- If you want to start over, press `Escape`. This will clear the text
- If you press escape again with no text, it will close Lazespell