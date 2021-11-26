#include <iostream>
#include <fstream>

using namespace std;

string username, hostname, bootPartitionPath, rootPartitionPath, swapPartitionPath, diskPath;

int createInstallScript(); int runScript();

int main()
{
    cout << "Hello world!" << endl;
    cout << "Specify path to destination disk: ";
    cin >> diskPath;
    cout << "Specify path to your root partition: ";
    cin >> rootPartitionPath;
    cout << "Specify path to your swap partition: ";
    cin >> swapPartitionPath;
    cout << "Specify path to your boot partition: ";
    cin >> bootPartitionPath;
    cout << "Specify hostname for your PC: ";
    cin >> hostname;
    cout << "Specify username for new user: ";
    cin >> username;

    createInstallScript();

    //system("chmod +x install.sh");
    //system("./install.sh");
    //system("rm install.sh")

    runScript();

    return 0;
}

int createInstallScript()
{
    ofstream outputScript("install.sh");

    //formating partitions

    outputScript << "mkfs.ext4 " << rootPartitionPath << endl; //formating root
    outputScript << "mkfs.ext4 " << bootPartitionPath << endl; //formating boot
    outputScript << "mkswap " << swapPartitionPath << endl; //formating swap

    //mounting partitions

    outputScript << "mount " << rootPartitionPath << " /mnt" << endl; //mounting root
    outputScript << "mkdir /mnt/boot /mnt/var /mnt/home" << endl;
    outputScript << "mount " << bootPartitionPath << " /mnt/boot" << endl; //mounting boot

    outputScript << "pacman -Syy" << endl << "pacstrap /mnt base base-devel linux linux-firmware nano dhcpcd net-tools grub" << endl << "genfstab -U /mnt >> /mnt/etc/fstab" << endl << "cp installRoot.sh /mnt/installRoot.sh" << endl << "chmod +x /mnt/installRoot.sh" << endl << "arch-chroot /mnt ./installRoot.sh" << endl << "rm /mnt/installRoot.sh" << endl << "umount /mnt/boot" << endl << "umount /mnt" << endl;

    outputScript.close();

    ofstream chrootSetupScript("installRoot.sh");

    chrootSetupScript << "en_US.UTF-8 UTF-8 >> /etc/locale.gen" << endl << "locale-gen" << endl << "echo LANG=en_US.UTF-8 > /etc/locale.conf" << endl << "export LANG=en_US.UTF-8" << "ln -s /usr/share/zoneinfo/Europe/Warsaw /etc/localtime" << endl << "hwclock --systohc --utc" << endl;
    chrootSetupScript << hostname << " > /etc/hostname" << endl << "systemctl enable dhcpcd" << endl << "echo Wpisz hasło dla root-a: " << endl << "passwd" << endl << "useradd -m -g users -G wheel -s /bin/bash " << username << endl << "echo Wpisz hasło dla użytkownika " << username << ": " << endl;
    chrootSetupScript << "passwd " << username << endl << "echo '" << username << " ALL=(ALL) ALL' >> /etc/sudoers" << endl << "grub-install " << diskPath << endl << "grub-mkconfig -o /boot/grub/grub.cfg" << endl << "mkinitcpio -p linux" << endl;

    return 0;
}

int runScript()
{
    string install = 0;
    system("chmod +x install.sh");
    do
    {
        system("clear");
        cout << "Installing to:\n\tRoot Partition:\t" << rootPartitionPath << "\n\tBoot Partition:\t" << bootPartitionPath << "\n\tSwap Partition:\t" << swapPartitionPath << "\nAre you sure you want to install Arch on this drive?[Y/N]";
        cin >> install;
    }while(install != "Y" && install != "N");
    if(install == "Y")
    {
        system("./install.sh");
        cout << "Installer Finished." << endl;
    }
    else
        cout << "Install cancelled." << endl;
    //system("rm install.sh installRoot.sh");

    return 0;
}
