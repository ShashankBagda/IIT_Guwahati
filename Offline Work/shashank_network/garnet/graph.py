import matplotlib.pyplot as plt
import subprocess
import numpy as np

# Command to be executed
command1 = "cd /home/shashank/Desktop/gem5/"

# Run the command and capture the output
output1 = subprocess.check_output(command1, shell=True, text=True)

# Print the output
print(output1)

# # Command to be executed
# command = "scons build/NULL/gem5.debug PROTOCOL=Garnet_standalone"

# # Run the command and capture the output
# output = subprocess.check_output(command, shell=True, text=True)

# # Print the output
# print(output)
with open('throughput.txt', 'w') as file:
            file.write("")
with open('throughput1.txt', 'w') as file:
            file.write("")

rate = 0.01
increment = 0.01
decimal = 2
x = np.array([])

while rate <= 1.0:
    command2 = "./build/X86/gem5.debug configs/example/garnet_synth_traffic.py  \
        --num-cpus=64 \
        --num-dirs=64 \
        --network=garnet \
        --topology=Mesh_XY \
        --mesh-rows=8  \
        --sim-cycles=1000000 \
        --synthetic=uniform_random \
        --injectionrate=" + str(rate)
    
    print(command2)
    print(rate)
    x = np.append(x, rate)
    rate += increment
    rate = round(rate,decimal)
    
    # Run the command and capture the output
    output = subprocess.check_output(command2, shell=True, text=True)

    # Print the output
    print(output)
    
    file = open("infected.txt", "r")
    trojans = file.read()
    file.close()
    print(trojans)

    injected = None
    received = None
    throughput = None
    
    with open('/home/shashank/Desktop/gem5/m5out/stats.txt', 'r') as file:
        for line in file:
            if line.startswith('system.ruby.network.packets_injected::total'):
                injected = line.split()[1]
                break
        for line in file:
            if line.startswith('system.ruby.network.packets_received::total'):
                received = line.split()[1]
                break
    if injected is not None and received is not None:
        
        without_t = int(received) / int(injected)
        rounded = round(without_t, 6)
        
        # Open the file in write mode            
        with open('throughput1.txt', 'a') as file:
            file.write(str(rounded))
            file.write("\n")
        
        with_t = int(received) - int(trojans)
        
        calculate = with_t / int(injected)
        throughput = round(calculate, 6)
        
        print(f"The injected after 'system.ruby.network.packets_injected::total' is: {injected}")
        print(f"The received after 'system.ruby.network.packets_received::total' is: {received}")
        print("The Throughput is : ", throughput)
        
        # Open the file in write mode            
        with open('throughput.txt', 'a') as file:
            file.write(str(throughput))
            file.write("\n")
    else:
        print("Target string not found in the file.")
    
    
# BUILDING THE GRAPH FROM DATA FILE
# Create a NumPy array
    
# Read data from the text file
y = np.loadtxt('throughput.txt')
z = np.loadtxt('throughput1.txt')

# Print the contents of the NumPy array
print(x)
print(y)
print(z)
    

# Plot the data
plt.plot(x, y, label="With Trojan")
plt.plot(x, z, label="Without Trojan")

# Customize the graph
plt.title("Graph of Packet Integrity Attack")
plt.xlabel("Injection Rate")
plt.ylabel("Throughput")

plt.legend()

# Display the graph
plt.show()