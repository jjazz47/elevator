#!/bin/bash

# This script installs pre-commit hook onto the elevator .git repostory.
# Author: jjazz47
# Last updated: 13/05/18
# Credit: Stefan Hajnoczi
#         (blog.vmsplice.net/2011/03/how-to-automatically-run-checkpatchpl.html)

REPO_PATH="../"
PRECOMMIT_FILE=".git/hooks/pre-commit"

echo "Installing Pre-commit hook..."

cat <<EOF > $REPO_PATH/$PRECOMMIT_FILE
#!/bin/bash

if [ "\$1" \= "selfcheck" ]; then
	echo "Installation: OKAY"
	exit 0
fi;

exec git diff --cached | scripts/checkpatch.pl --no-tree --no-signoff -q

EOF

echo $'Validating Pre-commit hook...\n'

if [ -e $REPO_PATH/$PRECOMMIT_FILE ]; then
	chmod +x  $REPO_PATH/$PRECOMMIT_FILE
	sh $REPO_PATH/$PRECOMMIT_FILE selfcheck
	if [ $? -eq 0 ]; then
		echo $'Validation Complete!\nInstallation Complete\n'
	else
		echo "pre-commit file is corrupted..."
		echo "Please re-run the installation script"
	fi;
else
	echo "pre-commit files does not exist..."
	echo "Please re-run the installation scirpt"
fi;

echo "Exiting pre-commit hook installation script"

