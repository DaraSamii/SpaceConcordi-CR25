import os
import pyvista as pv

def process_vtm(directory, results_path):
    """
    Processes a VTK dataset from a given directory, extracts contours for specified fields,
    and saves the results to the results_path.

    Args:
        directory (str): The directory containing the `VTK` folder.
        results_path (str): The directory to save the contour plots.
    """
    # Start a virtual frame buffer to avoid X server issues
    pv.start_xvfb()

    # Ensure results_path exists
    os.makedirs(results_path, exist_ok=True)
    print(f"Results path created: {results_path}")

    # Locate the VTK folder
    vtk_folder = os.path.join(directory, "VTK")
    if not os.path.exists(vtk_folder):
        raise FileNotFoundError(f"No VTK folder found in {directory}.")
    print(f"VTK folder located: {vtk_folder}")

    # Locate the .vtm file (excluding .series files)
    vtm_files = [f for f in os.listdir(vtk_folder) if f.endswith(".vtm") and not f.endswith("vtm.series")]
    if not vtm_files:
        raise FileNotFoundError("No .vtm file found in the VTK folder.")
    print(f"VTM files found: {vtm_files}")

    vtm_file = os.path.join(vtk_folder, vtm_files[0])  # Use the first valid .vtm file
    print(f"Using VTM file: {vtm_file}")

    # Load the .vtm file
    reader = pv.XMLMultiBlockDataReader(vtm_file)
    dataset = reader.read()

    # Process each block in the dataset
    for i, block in enumerate(dataset):
        if block is None:
            print(f"Skipping empty block {i}")
            continue
        print(f"Processing block {i}")

        block_index = 0
        block = mesh[block_index]  # Access the specific block

        # Create a slicing plane with a normal in the y-direction at (0, 0, 0)
        plane = block.slice(normal=(0, 1, 0), origin=(0, 0, 0))
        print(f"Slicing plane created for block {i}")

        # Specify the fields to process
        fields = ["T", "p", "U", "rho"]  # Temperature, Pressure, Velocity, Density

        for field in fields:
            try: 
                if field in plane.array_names:  # Ensure the field exists in the dataset
                    print(f"Processing field '{field}' in block {i}")
                        # Create the contour plot
                    plotter = pv.Plotter(off_screen=True)
                    plotter.add_mesh(plane, scalars=field, cmap="jet", show_scalar_bar=True)
                    plotter.view_xy()

                    # Save the plot to the results_path
                    output_file = os.path.join(results_path, f"{field}_block{i}_contour.pdf")
                    plotter.save_graphic(output_file)
                    plotter.clear()  # Clear the plotter for the next field
                    print(f"Saved contour plot: {output_file}")
                else:
                    print(f"Field '{field}' not found in block {i}")
            except Exception as e:
                print(f"Failed to process field '{field}' in block {i}: {e}")

    print(f"Contour plots saved to {results_path}")

# Example usage
process_vtm(directory="./Data/U210", results_path="./results")
