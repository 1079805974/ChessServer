const ROOM_EMPTY = "empty"
const ROOM_READY = "ready"
const ROOM_PLAYING = "playing"
const ROOM_W_WIN = "w_win"
const ROOM_B_WIN = "b_win"

var board = new Board(document.getElementById("container"), "images/", "sounds/");
board.setSearch(16);
board.millis = 10;
board.restart('');

function resetBoard(c){
    board.computer = c;
    board.restart('rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w');
}

var dispatch = []
dispatch['status'] = arr => {
    let roomId = arr[1]
    let status = arr[4]
    app.rooms[roomId].status = status
    app.rooms[roomId].host = arr[2]
    app.rooms[roomId].guest = arr[3]
    if (roomId == app2.thisRoom) {
        app2.redName = app.rooms[roomId].host
        app2.blackName = app.rooms[roomId].guest
        app2.status = status
    }
}

dispatch['move'] = arr => {
    app2.pauseThisClockAndRunOpp(!app2.thisRed)
    let mv = arr[1]
    board.addMove(mv, true)
}

dispatch['sync'] = arr => {
    let t1 = parseInt(arr[1])
    let t2 = parseInt(arr[2])
    let t3 = parseInt(arr[3])
    let t4 = parseInt(arr[4])
    app2.timeOffset = ((t2 - t1) + (t3 - t4)) / 2
}
var fromFen = fen => {
    board.pos.fromFen(fen)
    // if (fen.endsWith('b')) {
    //     if(board.pos.sdPlayer == 0){
    //         board.pos.changeSide()
    //     }
    // } else {
    //     if(board.pos.sdPlayer == 1){
    //         board.pos.changeSide()
    //     }
    // }
    board.flushBoard()
}

var toFen = () => {
    return board.pos.toFen()
}

function showBoard() {
    document.getElementById("board-div").style.display = "block"
}
function hideBoard() {
    document.getElementById("board-div").style.display = "none"
}
var roomNum = 0

var ws = new WebSocket("ws://localhost:3000")



var app = new Vue({
    el: '#app',
    data() {
        return {
            nickname: "Player" + Math.floor(Math.random() * 100),
            disableModify: true,
            rooms: [{
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }, {
                host: 'null',
                guest: 'null',
                status: 'empty'
            }],
            logined: false,
            inRoom: false
        }
    },
    watch:{
        inRoom(val){
            app2.inRoom  = val
        }
    },
    methods: {
        enterRoom(index) {
            if (!this.logined) {
                ws.send('login ' + this.nickname)
                app2.status = ROOM_EMPTY
            }
            if (this.rooms[index].status == ROOM_EMPTY) {
                ws.send('start ' + index)
                app2.status = ROOM_READY
            } else
                if (this.rooms[index].status == ROOM_READY) {
                    ws.send('enter ' + index)
                    app2.status = ROOM_PLAYING
                }
            showBoard()
            this.inRoom = true
            app2.thisRoom = index
            app2.start()
        },
        roomAvalible(rm) {
            return rm.status == ROOM_EMPTY || rm.status == ROOM_READY
        }
    },
    created() {
        hideBoard()
        ws.onopen = () => {
            ws.onmessage = function (msg) {
                console.log("receive msg: ", msg.data)
                let msgArr = msg.data.split(' ')
                dispatch[msgArr[0]](msgArr)
            }
            ws.onerror = console.log
            ws.send("status get")
        }
    }
})

var Clock1 = new Clock(1200)
var Clock2 = new Clock(1200)

var Clock3 = new Clock(90)
var Clock4 = new Clock(90)

Clock1.perSec = function(str){
    app2.RedClocks.gameClock = str
}

Clock3.perSec = function(str){
    app2.RedClocks.stepClock = str
    app2.redPercent = Math.floor(this.curTime / this.time * 100)
}

Clock2.perSec = function(str){
    app2.BlackClocks.gameClock = str
}
Clock4.perSec = function(str){
    app2.BlackClocks.stepClock = str
    app2.blackPercent = Math.floor(this.curTime / this.time * 100)
}


var app2 = new Vue({
    el: "#app2",
    data() {
        return {
            inRoom: app.inRoom,
            thisRoom: -1,
            status:'none',
            redName:'请等待',
            blackName:'请等待',
            startTime: 0,
            timeOffset: 0,   //server - client
            RedClocks: { gameClock: Clock1.toString(), stepClock: Clock3.toString() },
            BlackClocks: { gameClock: Clock2.toString(), stepClock: Clock4.toString() },
            sdPlayer: board.pos.sdPlayer,
            redPercent: 100,
            blackPercent: 100,
            thisRed: false
        }
    },
    watch:{
        status(val, old){
            switch(val){
                case ROOM_READY:{
                    if(old == ROOM_PLAYING){
                        console.log("taopaola")
                        onRunaway()
                        this.resetClocks()
                    }
                    fromFen('')
                    board.thinking.style.visibility = "visible";
                    this.thisRed = true
                    break;
                };
                case ROOM_PLAYING:{
                    board.thinking.style.visibility = "hidden";
                    if(this.thisRed){
                        resetBoard(1)
                        this.startRedCLock()
                    }else{
                        resetBoard(0)
                    }
                    break;
                }
            }
        }
    },
    methods: {
        ready(){
            board.restart('')
        },
        start() {
            //syncTime()
            this.startTime = new Date()
            //this.startRedCLock()
        },
        pauseThisClockAndRunOpp(thisRed){
            if(thisRed){
                this.pauseRedClock()
                this.startBlackCLock()
            }else{
                this.pauseBlackClock()
                this.startRedCLock()
            }
        },
        resetClocks(){
            Clock1.reset()
            Clock2.reset()
            Clock3.reset()
            Clock4.reset()
        },
        startRedCLock() {
            Clock1.start()
            Clock3.start()
        },
        pauseRedClock(){
            Clock1.pause()
            Clock3.reset()
            this.RedClocks.stepClock = Clock3.toString()
        },
        pauseBlackClock(){
            Clock2.pause()
            Clock4.reset()
            this.BlackClocks.stepClock = Clock4.toString()
        },
        redOverTime(func){
            Clock1.onEnd = func
            Clock3.onEnd = func
        },
        blackOverTime(func){
            Clock2.onEnd = func
            Clock4.onEnd = func
        },
        startBlackCLock() {
            Clock2.start()
            Clock4.start()
        },
        getColor(percent){
            if(percent > 0 && percent <= 33){
                return `rgb(255, 73, 73)`
            }else if(percent <= 60){
                return `rgb(32, 160, 255)`
            }else if(percent <= 100){
                return `rgb(19, 206, 102)`
            }
        }
    },
    created(){
        this.redOverTime(()=>onOverTime(this.thisRed))
        this.blackOverTime(()=>onOverTime(!this.thisRed))
    }
})

function onOverTime(side){
    if(app.inRoom){
        if(side){
            app.inRoom = false
            hideBoard()
            window.location.reload()
        }
        alert(side? "你已超时,被赶出房间！" : "对方已超时,被赶出房间！");
    }
}

function onRunaway() {  
    setTimeout(function() {
      alert("对方逃跑！你获得胜利！现在你是房主了～");
    }, 250);
  }

function sendMovePauseClock(msg){
    app2.pauseThisClockAndRunOpp(app2.thisRed)
    ws.send(msg)
}