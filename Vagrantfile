SERVER_NAME = "mli-ping-server"
SERVER_SCRIPT = <<-SHELL
  # Install GUI and Wireshark
  echo "wireshark-common wireshark-common/install-setuid boolean true" | sudo debconf-set-selections

  sudo apt-get update -y
  sudo DEBIAN_FRONTEND=noninteractive apt-get install -y \
    xfce4 \
    lightdm \
    wireshark \
    make gcc vim \
    inetutils-ping

  echo "cd /vagrant" >> /home/vagrant/.bashrc
  SHELL

Vagrant.configure("2") do |config|
  config.vm.box = "bento/debian-12.1"
  config.vm.box_url = "https://app.vagrantup.com/bento/boxes/debian-12.1/versions/202309.08.0/providers/virtualbox.box"

  config.vm.define SERVER_NAME do |server|
    server.vm.hostname = SERVER_NAME
    server.vm.provision "shell", inline: SERVER_SCRIPT
    server.vm.provider "virtualbox" do |vb|
      vb.name = SERVER_NAME
      vb.memory = 4096
      vb.cpus = 4
      vb.customize ["modifyvm", :id, "--vram", "96"]
    end
  end
end
