CWD=$(pwd)

echo "Starting Install..."
sleep 2
echo "Compiling Custom Commands..."
cd src/commands
chmod +x *
cd ../../
echo "Compiling BearShellC..."
gcc main.c -o BearShellC -lreadline
echo "Adding crucial directories to PATH..."
export PATH="$CWD":$PATH
export PATH=/bin:$PATH
export PATH=/usr/bin:$PATH