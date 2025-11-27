import csv
import sys
import os.path
import shutil
# Expects data to have columns:
# - cve_id
# - score
# - repo_url
# - hash

patchDir = "matched-patches"

class cve:
    cve_id: str
    score: int
    repo_url: str
    hash: str

    @staticmethod
    def FromCsvRow(row: dict):
        object = cve()

        object.cve_id = row["cve_id"]
        object.score = row["score"]
        object.repo_url = row["repo_url"]
        object.hash = row["hash"]

        return object

    def GetCommitUrl(this: csv) -> str:
        return f"{this.repo_url}/commit/{this.hash}"


def WriteUrls(inPath: str, outPath: str, patchesPath: str) -> None:
    with open(inPath, "r") as inFile:
        csvFile = csv.DictReader(inFile)

        with open(outPath, "w") as outFile:
            outFile.write("cve_id,score,commit_url,patch_file\n")

            for row in csvFile:
                cveObject = cve.FromCsvRow(row)
                commitUrl = cveObject.GetCommitUrl()
                patchFile = copyPatchFile(cveObject, patchesPath)                
                outFile.write(f"{cveObject.cve_id},{cveObject.score},{commitUrl}, {patchFile}\n")

def copyPatchFile(cveObject: cve, patchesPath: str):

    allPatches = os.listdir(patchesPath)

    for patchFile in allPatches:
        if cveObject.hash in patchFile:
            src = os.path.join(patchesPath, patchFile)
            dst = os.path.join(patchDir, patchFile)
            shutil.copy2(src, dst)
            return patchFile

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Please supply an input file path")
        exit(1)

    if len(sys.argv) < 3:
        print("Please supply an output file path")
        exit(1)

    if len(sys.argv) < 4:
        print("Please supply the patches folder path")
        exit(1)


    inPath = sys.argv[1]
    outPath = sys.argv[2]
    patchesPath = sys.argv[3]
    
    if not os.path.isfile(inPath):
        print(f"Could not find file \"{inPath}\"")
        exit(1)
    
    if os.path.isfile(outPath):
        print(f"Output file already exist \"{outPath}\"")
        exit(1)

    if not os.path.isdir(patchesPath):
        print(f'Could not find folder \"{patchesPath}\"')
        exit(1)

    os.mkdir(patchDir)
    if not os.path.isdir(patchDir):
        print(f'Could not find folder \"{patchDir}\"')
        exit(1)

    WriteUrls(inPath, outPath, patchesPath)
