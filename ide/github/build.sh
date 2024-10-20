echo "Check git variables"
echo "CurrDir = $(pwd)"
# echo "github.workspace = ${{ github.workspace }}"
echo "GITHUB_WORKSPACE = $GITHUB_WORKSPACE"
echo "GITHUB_ENV = $GITHUB_ENV"
echo "--------------------"
echo "cat $GITHUB_ENV"
## cat ${GITHUB_ENV}
echo "--------------------"
echo "GITHUB_REPO = $GITHUB_REPOSITORY"
echo "GITHUB_SHA = $GITHUB_SHA"
## echo ${GITHUB_SHA:0:7}
## echo .
# Bad substitution: GIT_HASH=$(echo ${GITHUB_SHA:0:7} )
GIT_HASH=$(echo $GITHUB_SHA | head -c 7)
echo "GIT_HASH=$GIT_HASH" >> $GITHUB_ENV
echo "GIT_HASH = $GIT_HASH"
