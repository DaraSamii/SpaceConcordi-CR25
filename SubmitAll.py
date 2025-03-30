import os
import shutil
import re
import subprocess

#===================================================================================================
def copy_and_modify_case(original_dir, data_dir, copied_folder, modifications, job_name):
    """
    Copies the OpenFOAM case directory, modifies parameters, updates submit.sh, and submits the job.
    
    :param original_dir: Path to the original directory
    :param data_dir: Path to the data directory where the copied folder will reside
    :param copied_folder: Name of the copied folder
    :param modifications: Dictionary of variable names and their new values
    :param job_name: The job name to set in the submit.sh script
    """
    # Define paths
    destination_path = os.path.join(data_dir, copied_folder)
    parameters_file = os.path.join(destination_path, "system", "parameters.cs")
    submit_script = os.path.join(destination_path, "submit.sh")
    initial_dir = os.getcwd()

    # Step 1: Copy the directory
    if os.path.exists(destination_path):
        shutil.rmtree(destination_path)
    shutil.copytree(original_dir, destination_path)
    print(f"Copied '{original_dir}' to '{destination_path}'.")

    # Step 2: Modify parameters file
    if not os.path.exists(parameters_file):
        raise FileNotFoundError(f"Parameters file not found: {parameters_file}")

    with open(parameters_file, 'r') as file:
        lines = file.readlines()

    for variable, new_value in modifications.items():
        param_pattern = re.compile(rf'^\s*{variable}\s+([\d.eE+-]+);')
        for i, line in enumerate(lines):
            if param_pattern.match(line):
                lines[i] = re.sub(param_pattern, f"{variable}        {new_value};", line)
                print(f"Updated '{variable}' to {new_value} in {parameters_file}.")
                break
        else:
            raise ValueError(f"Variable '{variable}' not found in parameters file.")

    with open(parameters_file, 'w') as file:
        file.writelines(lines)

    # Step 3: Modify submit.sh
    if not os.path.exists(submit_script):
        raise FileNotFoundError(f"Submit script not found: {submit_script}")

    with open(submit_script, 'r') as file:
        submit_lines = file.readlines()

    for i, line in enumerate(submit_lines):
        if line.startswith("#SBATCH --job-name="):
            submit_lines[i] = f"#SBATCH --job-name={job_name}\n"
            print(f"Updated job name in submit.sh to '{job_name}'.")
            break

    with open(submit_script, 'w') as file:
        file.writelines(submit_lines)

    # Step 4: Submit the job
    try:
        os.chdir(destination_path)
        print(f"Submitting job from directory: {os.getcwd()}")

        # Run sbatch
        result = subprocess.run(["sbatch", "submit.sh"], capture_output=True, text=True)
        print(f"Command executed: sbatch submit.sh")
        print(f"Return code: {result.returncode}")
        print(f"Stdout: {result.stdout}")
        print(f"Stderr: {result.stderr}")

        if result.returncode == 0:
            print(f"Job submitted successfully. Output:\n{result.stdout}")
        else:
            print(f"Failed to submit job. Error:\n{result.stderr}")
    except Exception as e:
        print(f"Error while submitting job: {e}")
    finally:
        os.chdir(initial_dir)

#===================================================================================================


original_dir = "./original"
data_dir = "./Data1"

# Define the parameter sets explicitly
parameter_sets = [
    {"Ux": 273.25, "Uy": 6.94, "Uz": 0.0, "P": 99066.1, "T": 289.367},
    {"Ux": 342, "Uy": 6.943344, "Uz": 0.0, "P": 97708.9, "T": 288.608},
    {"Ux": 408, "Uy": 12.5, "Uz": 0.0, "P": 96115.5, "T": 287.7},
    {"Ux": 721, "Uy": 26.66, "Uz": 0.0, "P": 78764.1, "T": 277.01},
]

# Loop over parameter sets
for params in parameter_sets:
    copied_folder = f"Ux{params['Ux']}Uy{params['Uy']}"
    job_name = copied_folder
    copy_and_modify_case(original_dir, data_dir, copied_folder, params, job_name)

