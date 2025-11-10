# Add at32f403a-407-docs as submodule to TafcoMcuCore
# Run this AFTER successfully pushing docs to GitHub

param(
    [Parameter(Mandatory=$true)]
    [string]$GithubUsername
)

Write-Host "================================" -ForegroundColor Cyan
Write-Host "Add Docs as Submodule" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

$submoduleUrl = "https://github.com/$GithubUsername/at32f403a-407-docs.git"
$docsPath = "Docs"

Write-Host "[1/5] Checking current location..." -ForegroundColor Yellow
$currentLocation = Get-Location
Write-Host "      Current: $currentLocation" -ForegroundColor Gray

if (-not (Test-Path ".git")) {
    Write-Host "      [ERROR] Not in TafcoMcuCore root (no .git folder)" -ForegroundColor Red
    Write-Host "      Please run from: D:\Repos\TafcoPigstore\TafcoMcuCore" -ForegroundColor Yellow
    exit 1
}

Write-Host "      [OK] In TafcoMcuCore root" -ForegroundColor Green
Write-Host ""

Write-Host "[2/5] Checking if Docs exists..." -ForegroundColor Yellow
if (Test-Path $docsPath) {
    Write-Host "      Docs folder exists, checking if it's already a submodule..." -ForegroundColor Gray
    
    $isSubmodule = Test-Path "$docsPath/.git"
    if ($isSubmodule) {
        Write-Host "      Docs is already a git repository" -ForegroundColor Gray
        Write-Host "      Removing current Docs folder..." -ForegroundColor Yellow
    }
    
    Write-Host "      Removing Docs folder..." -ForegroundColor Gray
    Remove-Item -Recurse -Force $docsPath
    Write-Host "      [OK] Removed" -ForegroundColor Green
} else {
    Write-Host "      [OK] Docs folder doesn't exist" -ForegroundColor Green
}

Write-Host ""

Write-Host "[3/5] Adding submodule..." -ForegroundColor Yellow
Write-Host "      URL: $submoduleUrl" -ForegroundColor Gray

try {
    git submodule add $submoduleUrl $docsPath
    if ($LASTEXITCODE -ne 0) {
        Write-Host "      [ERROR] Failed to add submodule" -ForegroundColor Red
        Write-Host ""
        Write-Host "Common issues:" -ForegroundColor Yellow
        Write-Host "  - Repository doesn't exist on GitHub" -ForegroundColor Gray
        Write-Host "  - Repository is private (must be public)" -ForegroundColor Gray
        Write-Host "  - Username is incorrect" -ForegroundColor Gray
        exit 1
    }
} catch {
    Write-Host "      [ERROR] Failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host "      [OK] Submodule added" -ForegroundColor Green
Write-Host ""

Write-Host "[4/5] Initializing submodule..." -ForegroundColor Yellow
git submodule update --init --recursive
Write-Host "      [OK] Initialized" -ForegroundColor Green
Write-Host ""

Write-Host "[5/5] Committing submodule..." -ForegroundColor Yellow
git add .gitmodules $docsPath

$commitMessage = @"
chore: convert Docs to submodule at32f403a-407-docs

Replace inline Docs folder with separate documentation repository.

Benefits:
- Independent versioning for documentation
- Optimized for Context7 indexing
- Separate issue tracking for doc improvements
- Easier community contributions
- Can be referenced by other projects

Submodule repository: https://github.com/$GithubUsername/at32f403a-407-docs
Documentation includes:
- ES0002 v2.0.11 complete errata (41/41 issues)
- 4 Mermaid diagrams
- 15+ code examples
- Quick reference tables
- Development checklists
"@

git commit -m $commitMessage

if ($LASTEXITCODE -eq 0) {
    Write-Host "      [OK] Committed" -ForegroundColor Green
} else {
    Write-Host "      [WARNING] Commit may have failed" -ForegroundColor Yellow
}

Write-Host ""

Write-Host "================================" -ForegroundColor Green
Write-Host "SUCCESS!" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Green
Write-Host ""
Write-Host "Submodule added successfully!" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Push TafcoMcuCore: git push origin main" -ForegroundColor Gray
Write-Host "  2. Verify Docs folder contains all files" -ForegroundColor Gray
Write-Host "  3. Check .gitmodules file was created" -ForegroundColor Gray
Write-Host ""
Write-Host "Submodule info:" -ForegroundColor Cyan
git submodule status
Write-Host ""

