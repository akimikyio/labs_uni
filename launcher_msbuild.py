import subprocess


projects = {
    "1": {
        "name": "Fireworks",
        "sln": "Fireworks\\rgr_solution\\Project.vcxproj",
        "exe": "Fireworks\\rgr_solution\\x64\\Release\\Project.exe"
    },
    "2": {
        "name": "Fishes",
        "sln": "Project1\\Project1.sln",
        "exe": "Project1\\x64\\Release\\Project1.exe"
    },
    "3": {
        "name": "Flyes",
        "sln": "RGR\\RGR.sln",
        "exe": "RGR\\x64\\Release\\RGR.exe"
    }
}

def build_project(sln_path):
    print("Сборка...")
    
    # Флаги для сборки: Релиз и 64-битная архитектура
    build_args_64 = ["/p:Configuration=Release", "/p:Platform=x64"]
    build_args_86 = ["/p:Configuration=Release", "/p:Platform=x86"]
    
    try:
        if "Fireworks" in sln_path or "Project1":
            result = subprocess.run(
                ["C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe", sln_path] + build_args_64,
            )
        else:
            result = subprocess.run(
                ["C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe", sln_path] + build_args_86,
            )
    except FileNotFoundError: 
        try:
            if "Fireworks" in sln_path:
                result = subprocess.run(
                    ["C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe", sln_path] + build_args_64,
            )
            else:
                result = subprocess.run(
                    ["C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe", sln_path] + build_args_86,
            )
        except FileNotFoundError:
            print("Ошибка: MSBuild не найден ни по одному из путей.")
            return False

    if result.returncode != 0:
        print("Ошибка сборки: \n")
        print(result.stdout)
        print(result.stderr)
        return False
    
    print("Сборка завершена успешно")
    return True

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

        if build_project(project["sln"]):
            run_exe(project["exe"])

