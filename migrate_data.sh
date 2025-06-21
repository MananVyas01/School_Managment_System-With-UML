#!/bin/bash

# Data Migration Script for School Management System
# This script migrates existing data files to the new data directory structure

echo "🔄 Starting data migration..."

# Create new directory structure
mkdir -p data
mkdir -p reports
mkdir -p backup

# Get current date for backup
BACKUP_DATE=$(date +"%Y%m%d_%H%M%S")
BACKUP_DIR="backup/migration_$BACKUP_DATE"
mkdir -p "$BACKUP_DIR"

# Function to migrate file
migrate_file() {
    local file=$1
    local target_dir=$2
    
    if [ -f "$file" ]; then
        echo "📄 Migrating $file..."
        # Create backup
        cp "$file" "$BACKUP_DIR/"
        # Move to new location
        mv "$file" "$target_dir/"
        echo "✅ $file migrated successfully"
    else
        echo "⚠️  $file not found, creating empty file in $target_dir/"
        touch "$target_dir/$file"
    fi
}

# Migrate data files
echo "📦 Migrating data files..."
migrate_file "students.txt" "data"
migrate_file "teachers.txt" "data"
migrate_file "courses.txt" "data"
migrate_file "classrooms.txt" "data"

# Clean up old executable if exists
if [ -f "School-Manage.exe" ]; then
    echo "🗑️  Moving old executable to backup..."
    mv "School-Manage.exe" "$BACKUP_DIR/"
fi

# Create .gitignore if it doesn't exist
if [ ! -f ".gitignore" ]; then
    echo "📝 Creating .gitignore..."
    cat > .gitignore << EOF
# Build files
obj/
bin/
*.o
*.exe

# Data files (optional - remove if you want to track data)
data/*.txt

# Reports
reports/

# Backup files
backup/

# IDE files
.vscode/
.idea/
*.swp
*.swo

# OS files
.DS_Store
Thumbs.db
EOF
fi

echo "✨ Data migration completed!"
echo "📁 Backup created in: $BACKUP_DIR"
echo "🏗️  You can now run 'make setup' to ensure all directories are ready"
echo "🧪 Run 'make test' to verify everything is working"
