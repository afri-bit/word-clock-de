var aaa = document.getElementById('aaa')
var bbb = document.getElementById('bbb')

aaa.onclick = function()
{
alert("clicked 123123123")
}

bbb.onclick = function()
{
    var request = new XMLHttpRequest()

    request.open('GET', '/get/color/background', true)
    request.onload = function () {
      // Begin accessing JSON data here
      var data = JSON.parse(this.response)
      alert(data.username)

//      if (request.status >= 200 && request.status < 400) {
//        data.forEach((movie) => {
//          console.log(movie.title)
//        })
//      } else {
//        console.log('error')
//      }
    }

    request.send()
}


