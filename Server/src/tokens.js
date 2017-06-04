var regTokens = [];

const getTokens = function(){ return regTokens; }
const addToken = function(token){
  if(regTokens.indexOf(token) == -1) regTokens.push(token);
}
const removeToken = function(token){
  const id = regTokens.indexOf(token);
  if(id != -1) regTokens.splice(id, 1);
}

module.exports = {
  getTokens,
  addToken,
  removeToken
}
