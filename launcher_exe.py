import subprocess


projects = {
    "1": {
        "name": "Fireworks",
        "exe": "Fireworks\\rgr_solution\\x64\\Release\\Project.exe"
    },
    "2": {
        "name": "Fishes",
        "exe": "Project1\\x64\\Release\\Project1.exe"
    },
    "3": {
        "name": "Flyes",
        "exe": "RGR\\x64\\Release\\RGR.exe"
    }
}


def run_exe(exe_path):
    subprocess.run([exe_path])

while True:
    print("\n==== Файлы АВТ-552 ====")
    print("1 - Fireworks")
    print("2 - Fishes")
    print("3 - Flyes")
    print("0 - Exit")

    choice = input("Введите номер: ")
    if choice == "0":
        break

    if choice in projects:
        project = projects[choice]
        run_exe(project["exe"])


