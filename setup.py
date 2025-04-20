from pathlib import Path
from setuptools import setup

def read_version():
    VERSION_FILENAME = "VERSION"
    VERSION_FILE_PATH = Path(__file__).parent / VERSION_FILENAME
    return VERSION_FILE_PATH.read_text().strip()

setup(
    version=read_version(),
)
