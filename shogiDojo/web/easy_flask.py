from flask import Flask, render_template, url_for, request

app = Flask(__name__)

@app.route('/')
def index():
	return render_template('test.html')

@app.route('/', methods=['POST'])
def getinput():
	data = request.data
	print(data)
	return 'hey'
