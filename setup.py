from setuptools import setup, find_packages

def get_requirements(filename):
    requirements = []
    with open(filename, "rt") as req_file:
        for line in req_file.read().splitlines():
            if not line.strip().startswith("#"):
                requirements.append(line)
    return requirements


project_requirements = get_requirements("requirements.txt")

setup(
    name='word-clock-controller',
    version='1.0.0',
    url="https://github.com/afri-bit/word-clock-de",
    license="MIT",
    author="Afrizal Herlambnag",
    author_email="afri.bit@outlook.com",
    description="Advanced german word clock.",
    
    packages=find_packages()
    
    # List run-time dependencies here.  These will be installed by pip when
    # your project is installed. For the detail information please refer to
    # https://packaging.python.org/en/latest/requirements.html
    install_requires=project_requirements,

    # Project relation
    keywords=["word clock", "wordclock", "Raspberry Pi", "raspi"]
    
    entry_points={
        "console_scripts": [
            "wcctrl=wcctrl.app_controller:main"
            "wcserver=wcctrl.app_server:main"
        ]
    }
)
