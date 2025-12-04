# Submission Guide

## Deadline: December 4, 2024 (before midnight on Canevas)

## Required Deliverables

### 1. PDF Report (3-4 pages) ✓

**File**: `PROJECT_REPORT.md` (needs to be converted to PDF)

**Contents** (all included):
- ✓ Graph modelling + assumptions + justification of edge weights
- ✓ Detailed attack scenario (malware description, impacted asset, attacker profile)
- ✓ Justification of at least 4 selected graph/tree algorithms
- ✓ Reference to research paper on graph theory in vehicular networks
- ✓ Sample input-output

### 2. C Project Files ✓

**Files included**:
- ✓ main.c
- ✓ Vehicule.h / Vehicule.c
- ✓ Simulation.h / Simulation.c
- ✓ graphalgos.h / graphalgos.c
- ✓ config.h
- ✓ README.md

**Code quality**:
- ✓ Clean, well-commented code
- ✓ Proper header files
- ✓ Acknowledgment of external sources (in report)

## Converting Report to PDF

### Option 1: Using Pandoc (Recommended)

```bash
# Install pandoc if not already installed
# Ubuntu/Debian:
sudo apt-get install pandoc texlive-latex-base texlive-fonts-recommended texlive-latex-extra

# Convert markdown to PDF
pandoc PROJECT_REPORT.md -o PROJECT_REPORT.pdf --pdf-engine=pdflatex -V geometry:margin=1in
```

### Option 2: Using Online Converter

1. Go to https://www.markdowntopdf.com/
2. Upload `PROJECT_REPORT.md`
3. Download the generated PDF
4. Rename to `PROJECT_REPORT.pdf`

### Option 3: Using VS Code Extension

1. Install "Markdown PDF" extension in VS Code
2. Open `PROJECT_REPORT.md`
3. Right-click → "Markdown PDF: Export (pdf)"

### Option 4: Using GitHub/GitLab

1. Push the markdown file to GitHub/GitLab
2. View the file in browser
3. Print to PDF using browser's print function

## Creating the Submission ZIP

### Automated Method (Linux/Mac/WSL)

```bash
# Run this from the project directory
zip -r V2V_Project_Submission.zip \
    main.c \
    Vehicule.h Vehicule.c \
    Simulation.h Simulation.c \
    graphalgos.h graphalgos.c \
    config.h \
    README.md \
    PROJECT_REPORT.pdf
```

### Manual Method (Any OS)

1. Create a new folder: `V2V_Project_Submission`
2. Copy these files into the folder:
   - main.c
   - Vehicule.h
   - Vehicule.c
   - Simulation.h
   - Simulation.c
   - graphalgos.h
   - graphalgos.c
   - config.h
   - README.md
   - PROJECT_REPORT.pdf (after conversion)
3. Right-click the folder → "Compress" or "Send to → Compressed folder"
4. Rename to `V2V_Project_Submission.zip`

## Final Checklist Before Submission

- [ ] PROJECT_REPORT.md converted to PROJECT_REPORT.pdf
- [ ] PDF is 3-4 pages long
- [ ] All C files compile without errors: `gcc -o simulation main.c Vehicule.c Simulation.c graphalgos.c -lm`
- [ ] Simulation runs and produces output: `./simulation`
- [ ] All required files are in the ZIP
- [ ] ZIP file is named appropriately
- [ ] Submitted to Canevas before midnight on December 4, 2024

## Demonstration Preparation (Dec. 5, afternoon)

### Requirements
- Teams of 2
- 5-minute demo
- One summary slide: modelled graph + key decisions
- Live run showing sample input/output

### Suggested Slide Content

**Title**: V2V Malware Propagation Simulator

**Graph Model**:
- Nodes: 50 vehicles with position, velocity, infection state
- Edges: Proximity-based (300-unit range), weighted by distance
- Dynamic topology: Graph changes as vehicles move

**Key Decisions**:
1. Edge weight = distance (realistic, efficient, multi-purpose)
2. Linear road model (simplicity vs. scalability)
3. Epidemic SIR model (proven infection dynamics)
4. Four graph algorithms (clusters, shortest path, MST, centrality)

**Results**:
- 90% infection rate by T=40
- Critical hubs identified (max degree: 10)
- Network clusters reveal containment zones

### Demo Script

1. **Show configuration** (config.h): 50 vehicles, 300 range, 0.10 transmission
2. **Run simulation**: `./simulation`
3. **Highlight key output**:
   - Infection events at various timestamps
   - Cluster analysis showing communities
   - Critical hub identification
   - Shortest path from Patient Zero
4. **Explain security implications**: Why these algorithms matter for defense

## Questions to Prepare For

1. Why did you choose distance as edge weight?
2. How do the graph algorithms help in security analysis?
3. What happens if you increase/decrease DANGER_RADIUS?
4. How scalable is your solution to larger networks?
5. What are the limitations of the linear road model?
6. How does the epidemic model relate to real malware?

## Good Luck!

Remember: The demonstration is as important as the code. Practice explaining your design decisions clearly and concisely.
