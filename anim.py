import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import io
import os
import sys

# Retrieve arguments from command line
if len(sys.argv) != 3:
    print("Usage: python anim.py <folder_path> <temperature>")
    sys.exit(1)

folder_path = sys.argv[1]
temperature = float(sys.argv[2])

# Configuration
step_range = range(0, 300000, 100)  # From step 0 to step 9900 in increments of 100
gif_filename = f'animation_{temperature:.1f}.gif'

# Function to create an image of the lattice
def create_lattice_frame(filename, step):
    data = np.loadtxt(filename)  # Load the data from the file
    L = int(np.max(data[:, 0])) + 1  # Assume lattice size is the maximum x coordinate + 1
    lattice = np.zeros((L, L), dtype=int)
    
    x = data[:, 0].astype(int)  # x-coordinates
    y = data[:, 1].astype(int)  # y-coordinates
    spin = data[:, 2]  # Spin values (1 or -1)
    
    # Fill the lattice array
    lattice[y, x] = spin
    
    fig, ax = plt.subplots(figsize=(8, 6))
    
    # Define a custom colormap with only two colors: black for spin -1 and yellow for spin 1
    cmap = plt.cm.colors.ListedColormap(['red', 'blue'])
    
    # Create an image of the lattice
    cax = ax.imshow(lattice, cmap=cmap, origin='upper', interpolation='none')
    ax.set_title(f'Lattice Configuration at T={temperature:.1f}, Step={step}')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_xticks([])
    ax.set_yticks([])
    
    # Save the frame to an in-memory buffer
    buf = io.BytesIO()
    plt.savefig(buf, format='png', bbox_inches='tight', pad_inches=0)
    plt.close(fig)
    buf.seek(0)  # Rewind the buffer to the beginning
    return buf

# Create the GIF animation
frames = []
for step in step_range:
    filename = os.path.join(folder_path, f'lattice_step{step}.txt')  # Construct the filename for the current step
    if os.path.exists(filename):  # Check if the file exists
        buf = create_lattice_frame(filename, step)  # Generate the frame
        frames.append(Image.open(buf))  # Append the frame to the list

# Save the frames as a GIF
if frames:
    frames[0].save(gif_filename, save_all=True, append_images=frames[1:], duration=100, loop=0)

print(f'Animation saved as {gif_filename}')
