if [ "$#" -ne 1 ]; then
  echo "./upload.sh <commit message>"
  exit 1
fi


git add .
git commit -m "$1"
git push -u origin main

exit 0
