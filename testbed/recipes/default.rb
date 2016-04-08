#
# Cookbook Name:: testbed
# Recipe:: default
#
# Copyright (C) 2015 YOUR_NAME
#
# All rights reserved - Do Not Redistribute
#
group "jpbarto" do
  action :create
end

user "jpbarto" do
  gid "jpbarto"
  home '/home/jpbarto'
  shell '/bin/bash'
  password '$6$UpV0OpsB$QsP/nva2wv48mfuv.Jz1qn.XpbsLs6P3J6jjDQ3CIOUACAie43WCo9.OMpxm0S53NzaeaUDHsdZCs2TCGg.o11'
end

directory "/home/jpbarto" do
  owner 'jpbarto'
  group 'jpbarto'
  mode '0750'
  action :create
end

group "sudo" do
  append true
  members 'jpbarto'
end

package "gcc" do
  action :install
end
