# Add Docs as Submodule - Complete Workflow

This guide shows how to convert the `Docs/` folder into a separate repository and add it as a submodule to TafcoMcuCore.

---

## ✅ Step 1: Repository Initialized (DONE)

The docs repository is ready:
- ✅ Git initialized in `TafcoMcuCore/Docs/`
- ✅ Initial commit created (`0ef4242`)
- ✅ Branch renamed to `main`
- ✅ 10 files committed (2,906+ insertions)

---

## 📋 Step 2: Create GitHub Repository

### Option A: Using GitHub Web Interface (Recommended)

1. **Go to:** https://github.com/new

2. **Fill in details:**
   ```
   Repository name: at32f403a-407-docs
   Description: 📚 Complete errata documentation for Artery AT32F403A/407 ARM Cortex-M4 MCUs | 41 issues with workarounds & code examples | Context7-optimized
   Public: ✅ YES
   Add README: ❌ NO
   Add .gitignore: ❌ NO
   Choose license: ❌ NO
   ```

3. **Click:** "Create repository"

4. **Copy the repository URL** (should be):
   ```
   https://github.com/YOUR-USERNAME/at32f403a-407-docs.git
   ```

### Option B: Using GitHub CLI (if installed)

```bash
cd D:\Repos\TafcoPigstore\TafcoMcuCore\Docs
gh repo create at32f403a-407-docs --public --source=. --remote=origin --push
```

---

## 🚀 Step 3: Push to GitHub

```bash
# Navigate to docs folder
cd D:\Repos\TafcoPigstore\TafcoMcuCore\Docs

# Add remote (REPLACE YOUR-USERNAME!)
git remote add origin https://github.com/YOUR-USERNAME/at32f403a-407-docs.git

# Push to GitHub
git push -u origin main
```

**Expected output:**
```
Enumerating objects: 13, done.
Counting objects: 100% (13/13), done.
Delta compression using up to X threads
Compressing objects: 100% (12/12), done.
Writing objects: 100% (13/13), XXX.XX KiB | X.XX MiB/s, done.
Total 13 (delta 0), reused 0 (delta 0), pack-reused 0
To https://github.com/YOUR-USERNAME/at32f403a-407-docs.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```

---

## 🔗 Step 4: Add as Submodule to TafcoMcuCore

Now that the docs repo is on GitHub, add it as a submodule:

```bash
# Go to TafcoMcuCore root (parent directory)
cd D:\Repos\TafcoPigstore\TafcoMcuCore

# IMPORTANT: Backup current Docs folder first (optional but recommended)
# copy -r Docs Docs_backup

# Remove current Docs folder
# PowerShell:
Remove-Item -Recurse -Force Docs

# Add as submodule (REPLACE YOUR-USERNAME!)
git submodule add https://github.com/YOUR-USERNAME/at32f403a-407-docs.git Docs

# Update submodule
git submodule update --init --recursive
```

**Expected output:**
```
Cloning into 'D:/Repos/TafcoPigstore/TafcoMcuCore/Docs'...
remote: Enumerating objects: 13, done.
remote: Counting objects: 100% (13/13), done.
remote: Compressing objects: 100% (12/12), done.
remote: Total 13 (delta 0), reused 13 (delta 0), pack-reused 0
Receiving objects: 100% (13/13), XXX.XX KiB | X.XX MiB/s, done.
```

---

## 💾 Step 5: Commit Submodule to TafcoMcuCore

```bash
# Still in TafcoMcuCore root
cd D:\Repos\TafcoPigstore\TafcoMcuCore

# Stage the submodule
git add .gitmodules Docs

# Commit the submodule configuration
git commit -m "chore: convert Docs to submodule at32f403a-407-docs

Replace inline Docs folder with separate documentation repository.

Benefits:
- Independent versioning for documentation
- Optimized for Context7 indexing
- Separate issue tracking for doc improvements
- Easier community contributions
- Can be referenced by other projects

Submodule repository: https://github.com/YOUR-USERNAME/at32f403a-407-docs
Documentation includes:
- ES0002 v2.0.11 complete errata (41/41 issues)
- 4 Mermaid diagrams
- 15+ code examples
- Quick reference tables
- Development checklists"

# Push to TafcoPigstore
git push origin main
```

---

## 📁 Expected Structure After Submodule

```
TafcoPigstore/
├── TafcoMcuCore/
│   ├── .gitmodules ← NEW: Submodule configuration
│   ├── Docs/ ← NOW A SUBMODULE (points to at32f403a-407-docs repo)
│   │   └── [all doc files from submodule]
│   ├── src/
│   └── ...
└── Docs/
    └── Artery/
        └── ...
```

### .gitmodules Content:
```ini
[submodule "Docs"]
	path = Docs
	url = https://github.com/YOUR-USERNAME/at32f403a-407-docs.git
```

---

## 🔄 Working with Submodules

### For You (Repository Owner):

**Clone TafcoPigstore with submodules:**
```bash
git clone --recursive https://github.com/YOUR-USERNAME/TafcoPigstore.git
```

**Update submodule to latest:**
```bash
cd D:\Repos\TafcoPigstore\TafcoMcuCore
git submodule update --remote Docs
git add Docs
git commit -m "chore: update docs submodule to latest"
git push
```

**Make changes to docs:**
```bash
# Go to submodule
cd D:\Repos\TafcoPigstore\TafcoMcuCore\Docs

# Make changes, commit, push
git add .
git commit -m "docs: add new example"
git push origin main

# Go back to parent repo
cd ..

# Update parent to track new submodule commit
git add Docs
git commit -m "chore: update docs submodule"
git push
```

### For Contributors:

**Clone with submodules:**
```bash
git clone --recursive https://github.com/YOUR-USERNAME/TafcoPigstore.git
```

**If already cloned without --recursive:**
```bash
cd TafcoPigstore/TafcoMcuCore
git submodule init
git submodule update
```

---

## ✅ Verification Checklist

After completing all steps:

**On GitHub:**
- [ ] `at32f403a-407-docs` repository created and public
- [ ] All files visible in the repository
- [ ] README displays correctly
- [ ] Mermaid diagrams render
- [ ] Images display properly

**On Local Machine:**
- [ ] `TafcoMcuCore/Docs` is now a submodule
- [ ] `.gitmodules` file exists in TafcoMcuCore root
- [ ] Can navigate to `TafcoMcuCore/Docs` and see all files
- [ ] `git status` in TafcoMcuCore shows clean state
- [ ] `git submodule status` shows correct commit

**Test Clone:**
```bash
# In a different directory
cd D:\Temp
git clone --recursive https://github.com/YOUR-USERNAME/TafcoPigstore.git test-clone
cd test-clone/TafcoMcuCore/Docs
ls  # Should show all doc files
```

---

## 🎯 Benefits of This Approach

### For Documentation Repository:
✅ Independent version control  
✅ Separate issue tracking  
✅ Context7-optimized structure  
✅ Can be starred/forked independently  
✅ Easier for community contributions  
✅ Referenced by multiple projects  

### For TafcoMcuCore:
✅ Keeps main repo focused on code  
✅ Docs stay in sync via submodule  
✅ Can pin to specific doc version  
✅ Cleaner repository structure  
✅ Docs can be updated independently  

---

## 🆘 Troubleshooting

### Error: "A git directory for 'Docs' is found locally"
```bash
cd TafcoMcuCore
git rm --cached Docs
rm -rf .git/modules/Docs
rm -rf Docs
git submodule add https://github.com/YOUR-USERNAME/at32f403a-407-docs.git Docs
```

### Error: "Docs is already tracked"
```bash
# Remove from git tracking first
git rm -r --cached Docs
git commit -m "chore: remove Docs from tracking"
# Then add as submodule
git submodule add https://github.com/YOUR-USERNAME/at32f403a-407-docs.git Docs
```

### Submodule shows "modified" but no changes
```bash
cd TafcoMcuCore
git submodule update
```

### Want to remove submodule?
```bash
cd TafcoMcuCore
git submodule deinit Docs
git rm Docs
git commit -m "chore: remove docs submodule"
```

---

## 📊 Summary

**What was done:**
1. ✅ Initialized git in `TafcoMcuCore/Docs/`
2. ✅ Created initial commit with all files
3. ✅ Renamed branch to `main`

**What you need to do:**
1. ⏳ Create GitHub repository `at32f403a-407-docs`
2. ⏳ Push docs to GitHub
3. ⏳ Add as submodule to TafcoMcuCore
4. ⏳ Commit and push TafcoMcuCore changes

**Time estimate:** 5-10 minutes

---

## 🚀 Quick Command Reference

```bash
# === STEP 1: Push Docs to GitHub ===
cd D:\Repos\TafcoPigstore\TafcoMcuCore\Docs
git remote add origin https://github.com/YOUR-USERNAME/at32f403a-407-docs.git
git push -u origin main

# === STEP 2: Add as Submodule ===
cd D:\Repos\TafcoPigstore\TafcoMcuCore
Remove-Item -Recurse -Force Docs  # PowerShell
git submodule add https://github.com/YOUR-USERNAME/at32f403a-407-docs.git Docs

# === STEP 3: Commit to TafcoMcuCore ===
git add .gitmodules Docs
git commit -m "chore: convert Docs to submodule"
git push origin main
```

---

**Ready to proceed?** Create the GitHub repo and run the commands! 🎉

