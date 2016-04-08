#
# Cookbook Name:: testbed
# Recipe:: linux-desktop
#
# Copyright (C) 2015 YOUR_NAME
#
# All rights reserved - Do Not Redistribute
#
#
#
case node["platform_family"]
when "debian"
  execute "update dpkg packages" do
    command "apt-get update"
  end

  log "completed update of aptitude packages"

  package "virtualbox-guest-x11" do
    action :install
  end

  include_recipe "xfce4"
when "rhel"
  package "xorg-x11-server-Xorg" do
    action :install
  end
  
  package "xorg-x11-xinit" do
    action :install
  end

  package "xorg-x11-drv-vesa" do
    action :install
  end

  package "xorg-x11-drv-evdev" do
    action :install
  end

  package "openmotif" do
    action :install
  end
end

package "xterm" do
  action :install
end

package "firefox" do
  action :install
end

include_recipe 'vim'
