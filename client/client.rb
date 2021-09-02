
require 'socket'

s = TCPSocket.new 'localhost', 12345

path="/home/amadou/Bureau/cheeroke/group-774242/clients/files/";

s.write(path+"#{ARGV[0]}.c\n")

s.each_line do |line|
    puts line
end

s.close