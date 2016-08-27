pushd gmusicapi
python setup.py sdist --formats=zip -k
for /f "tokens=*" %%a in ('dir dist\*.zip /b') do (
	pip install -v dist\%%a
)
popd