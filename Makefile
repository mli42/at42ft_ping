vm.up:
	vagrant up

vm.stop:
	vagrant halt

vm.reboot: vm.stop vm.up

vm.reload:
	vagrant reload

vm.ssh:
	vagrant ssh

vm.clean:
	vagrant destroy
