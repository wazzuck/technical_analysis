<html>
<head>
	<style>
	html, body {
	    margin: 0;
	    padding: 0;
	    overflow: hidden;
	}

	#svgPhoto {
	    position: fixed;
	    top: 0;
	    bottom: 0;
	    left: 0;
	    right: 0;
	    width: 100%;
	    height: 100vh;
	}
	</style>
</head>

<body>
	<img id=svgPhoto src="/static/{{ param }}"/>
</body>
</html>
