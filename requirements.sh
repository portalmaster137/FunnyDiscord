NEEDED_PACKAGES="libsodium-dev pkg-config libopus-dev zlib1g-dev libssl-dev"

# Check for root, and ask sudo if not root

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run as root. Please enter your password to continue."
    sudo -k
    sudo -v
    if [ $? -eq 1 ]; then
        echo "Sorry, try again."
        exit 1
    fi
fi

# Install packages

sudo apt update
sudo apt install -y $NEEDED_PACKAGES